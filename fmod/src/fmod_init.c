#include "fmod_ext.h"
#include "../include/fmod_errors.h"
#include <string.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/emscripten.h>
#endif

#ifdef __linux__
#include <sys/auxv.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <libgen.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#include <shlwapi.h>
#endif

FMOD_STUDIO_SYSTEM* FMODExt_system = NULL;
FMOD_SYSTEM* FMODExt_lowLevelSystem = NULL;
bool FMODExt_isPaused = false;

static bool runWhileIconified = false;
static bool iconified = false;
static FMOD_BOOL masterChannelGroupPaused;

static FMOD_SPEAKERMODE speakerModeFromString(const char* str) {
    if (0 == strcmp(str, "default")) { return FMOD_SPEAKERMODE_DEFAULT; }
    if (0 == strcmp(str, "stereo")) { return FMOD_SPEAKERMODE_STEREO; }
    if (0 == strcmp(str, "mono")) { return FMOD_SPEAKERMODE_MONO; }
    if (0 == strcmp(str, "5.1")) { return FMOD_SPEAKERMODE_5POINT1; }
    if (0 == strcmp(str, "7.1")) { return FMOD_SPEAKERMODE_7POINT1; }
    if (0 == strcmp(str, "quad")) { return FMOD_SPEAKERMODE_QUAD; }
    if (0 == strcmp(str, "surround")) { return FMOD_SPEAKERMODE_SURROUND; }
    if (0 == strcmp(str, "max")) { return FMOD_SPEAKERMODE_MAX; }
    if (0 == strcmp(str, "raw")) { return FMOD_SPEAKERMODE_RAW; }
    LOGW("Invalid value for speaker_mode: \"%s\". Using default", str);
    return FMOD_SPEAKERMODE_DEFAULT;
}

#define check(fcall)                                    \
    do {                                                \
        FMOD_RESULT res = fcall;                        \
        if (res != FMOD_OK) {                           \
            LOGE("%s", FMOD_ErrorString(res));          \
            FMOD_Studio_System_Release(FMODExt_system); \
            FMODExt_system = NULL;                      \
            return;                                     \
        }                                               \
    } while (0)

void FMODExt_init(lua_State* L) {
    if (!FMODExt_linkLibraries()) {
        LOGE("Failed to link FMOD libraries");
        return;
    }

    FMOD_RESULT res;
    res = FMOD_Studio_System_Create(&FMODExt_system, FMOD_VERSION);
    if (res != FMOD_OK) {
        LOGE("%s", FMOD_ErrorString(res));
        FMODExt_system = NULL;
        return;
    }

    check(FMOD_Studio_System_GetCoreSystem(FMODExt_system, &FMODExt_lowLevelSystem));

    int defaultSampleRate = 0;
    unsigned int bufferLength = 0;
    int numBuffers = 0;

#ifdef __EMSCRIPTEN__
    check(FMOD_System_GetDriverInfo(FMODExt_lowLevelSystem, 0, NULL, 0, NULL, &defaultSampleRate, NULL, NULL));
    bufferLength = 2048;
    numBuffers = 2;
#endif

    bufferLength = (unsigned int)FMODExt_dmConfigFile_GetInt("fmod.buffer_length", bufferLength);
    numBuffers = FMODExt_dmConfigFile_GetInt("fmod.num_buffers", numBuffers);

    if (bufferLength || numBuffers) {
        if (!bufferLength) { bufferLength = 1024; }
        if (!numBuffers) { numBuffers = 4; }
        check(FMOD_System_SetDSPBufferSize(FMODExt_lowLevelSystem, bufferLength, numBuffers));
    }

    int sampleRate = FMODExt_dmConfigFile_GetInt("fmod.sample_rate", defaultSampleRate);
    int numRawSpeakers = FMODExt_dmConfigFile_GetInt("fmod.num_raw_speakers", 0);
    const char* speakerModeStr = FMODExt_dmConfigFile_GetString("fmod.speaker_mode", "default");
    FMOD_SPEAKERMODE speakerMode = speakerModeFromString(speakerModeStr);

    if (sampleRate || numRawSpeakers || speakerMode != FMOD_SPEAKERMODE_DEFAULT) {
        check(FMOD_System_SetSoftwareFormat(FMODExt_lowLevelSystem, sampleRate, speakerMode, numRawSpeakers));
    }

    FMOD_STUDIO_INITFLAGS studioInitFlags = FMOD_STUDIO_INIT_NORMAL;
    if (FMODExt_dmConfigFile_GetInt("fmod.live_update", 0)) { studioInitFlags |= FMOD_STUDIO_INIT_LIVEUPDATE; }

    void* extraDriverData = NULL;
    check(FMOD_Studio_System_Initialize(FMODExt_system, 1024, studioInitFlags, FMOD_INIT_NORMAL, extraDriverData));

    FMODExt_isPaused = false;

    iconified = false;
    runWhileIconified = FMODExt_dmConfigFile_GetInt("fmod.run_while_iconified",
                                                    FMODExt_dmConfigFile_GetInt("engine.run_while_iconified", 0)) != 0;

#if TARGET_OS_IPHONE
    FMODExt_initIOSInterruptionHandler();
#endif

    FMODExt_register(L);
}

void FMODExt_update() {
    if (!FMODExt_system || FMODExt_isPaused) { return; }

    FMOD_RESULT res = FMOD_Studio_System_Update(FMODExt_system);
    if (res != FMOD_OK) {
        LOGE("%s", FMOD_ErrorString(res));
        FMOD_Studio_System_Release(FMODExt_system);
        FMODExt_system = NULL;
    }
}

void FMODExt_finalize() {
    if (FMODExt_system) {
        FMOD_RESULT res = FMOD_Studio_System_Release(FMODExt_system);
        if (res != FMOD_OK) { LOGE("%s", FMOD_ErrorString(res)); }
        FMODExt_system = NULL;
    }
    FMODExt_cleanupLibraries();
}

void FMODExt_resumeMixer() {
    if (FMODExt_system && FMODExt_isPaused) {
        check(FMOD_System_MixerResume(FMODExt_lowLevelSystem));
        FMODExt_isPaused = false;
    }
}

void FMODExt_suspendMixer() {
    if (FMODExt_system && !FMODExt_isPaused) {
        check(FMOD_System_MixerSuspend(FMODExt_lowLevelSystem));
        FMODExt_isPaused = true;
    }
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
__attribute__((used)) void FMODExt_unmuteAfterUserInteraction() {
    if (FMODExt_system && !FMODExt_isPaused) {
        check(FMOD_System_MixerSuspend(FMODExt_lowLevelSystem));
        check(FMOD_System_MixerResume(FMODExt_lowLevelSystem));
    }
}
#endif

void FMODExt_activateApp() {
#if defined(__EMSCRIPTEN__) || defined(__ANDROID__)
    FMODExt_resumeMixer();
#endif
}

void FMODExt_deactivateApp() {
#if defined(__EMSCRIPTEN__) || defined(__ANDROID__)
    FMODExt_suspendMixer();
#endif
}

void FMODExt_iconifyApp() {
    if (iconified) { return; }
    iconified = true;

    if (!runWhileIconified && FMODExt_lowLevelSystem) {
        FMOD_CHANNELGROUP* channelGroup;
        check(FMOD_System_GetMasterChannelGroup(FMODExt_lowLevelSystem, &channelGroup));
        check(FMOD_ChannelGroup_GetPaused(channelGroup, &masterChannelGroupPaused));
        check(FMOD_ChannelGroup_SetPaused(channelGroup, true));
    }
}

void FMODExt_deiconifyApp() {
    if (!iconified) { return; }
    iconified = false;

    if (!runWhileIconified && FMODExt_lowLevelSystem) {
        FMOD_CHANNELGROUP* channelGroup;
        check(FMOD_System_GetMasterChannelGroup(FMODExt_lowLevelSystem, &channelGroup));
        check(FMOD_ChannelGroup_SetPaused(channelGroup, masterChannelGroupPaused));
    }
}

#if !defined(__APPLE__)
int FMODExt_getBundleRoot(lua_State* L) {
#if defined(_WIN32)
    HMODULE hModule = GetModuleHandle(NULL);
    char path[MAX_PATH];
    GetModuleFileNameA(hModule, path, MAX_PATH);
    PathRemoveFileSpecA(path);
    lua_pushstring(L, path);
    return 1;
#elif defined(__linux__) && !defined(__ANDROID__)
    char* path = (char*)malloc(PATH_MAX + 2);
    ssize_t ret = readlink("/proc/self/exe", path, PATH_MAX + 2);
    if (ret >= 0 && ret <= PATH_MAX + 1) {
        lua_pushstring(L, dirname(path));
    } else {
        const char* path2 = (const char*)getauxval(AT_EXECFN);
        if (!path2) {
            lua_pushstring(L, ".");
            free(path);
            return 1;
        }
        if (!realpath(path2, path)) {
            lua_pushstring(L, ".");
            free(path);
            return 1;
        }
        lua_pushstring(L, dirname(path));
    }
    free(path);
    return 1;
#else
    lua_pushstring(L, ".");
    return 1;
#endif
}
#endif
