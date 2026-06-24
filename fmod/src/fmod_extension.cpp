#define LIB_NAME "DefoldFMOD"
#define MODULE_NAME "fmod"

#ifndef DLIB_LOG_DOMAIN
#define DLIB_LOG_DOMAIN "fmod"
#endif
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX) || \
    defined(DM_PLATFORM_IOS) || defined(DM_PLATFORM_HTML5) || defined(DM_PLATFORM_ANDROID) || \
    defined(DM_PLATFORM_SWITCH)

#include "fmod_ext.h"

#ifdef DM_PLATFORM_ANDROID
#include <dmsdk/dlib/android.h>
#endif

static dmConfigFile::HConfig appConfig;

dmExtension::Result AppInitializeDefoldFMOD(dmExtension::AppParams* params) {
    appConfig = params->m_ConfigFile;
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeDefoldFMOD(dmExtension::Params* params) {
#ifdef DM_PLATFORM_ANDROID
    dmAndroid::ThreadAttacher thread;
#endif
    FMODExt_init(params->m_L);
    return dmExtension::RESULT_OK;
}

dmExtension::Result UpdateDefoldFMOD(dmExtension::Params* params) {
#ifdef DM_PLATFORM_ANDROID
    dmAndroid::ThreadAttacher thread;
#endif
    FMODExt_update();
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeDefoldFMOD(dmExtension::Params* params) {
#ifdef DM_PLATFORM_ANDROID
    dmAndroid::ThreadAttacher thread;
#endif
    FMODExt_finalize();
    return dmExtension::RESULT_OK;
}

void OnEventDefoldFMOD(dmExtension::Params* params, const dmExtension::Event* event) {
#ifdef DM_PLATFORM_ANDROID
    dmAndroid::ThreadAttacher thread;
#endif
    switch (event->m_Event) {
    case EXTENSION_EVENT_ID_ACTIVATEAPP:
        FMODExt_activateApp();
        break;
    case EXTENSION_EVENT_ID_DEACTIVATEAPP:
        FMODExt_deactivateApp();
        break;
    case EXTENSION_EVENT_ID_ICONIFYAPP:
        FMODExt_iconifyApp();
        break;
    case EXTENSION_EVENT_ID_DEICONIFYAPP:
        FMODExt_deiconifyApp();
        break;
    default:
        break;
    }
}

int FMODExt_dmBuffer_GetBytes(FMODExt_HBuffer buffer, void** bytes, uint32_t* size) {
    return dmBuffer::GetBytes((dmBuffer::HBuffer)buffer, bytes, size) != dmBuffer::RESULT_OK;
}

void FMODExt_dmScript_PushBuffer(lua_State* L, FMODExt_HBuffer buffer) {
    dmScript::LuaHBuffer wrapper(buffer, dmScript::OWNER_C);
    dmScript::PushBuffer(L, wrapper);
}

FMODExt_HBuffer FMODExt_dmScript_CheckBuffer(lua_State* L, int index) {
    return dmScript::CheckBuffer(L, index)->m_Buffer;
}

void FMODExt_dmScript_PushVector3(lua_State* L, float x, float y, float z) {
    dmScript::PushVector3(L, Vectormath::Aos::Vector3(x, y, z));
}

FMOD_VECTOR FMODExt_dmScript_CheckVector3(lua_State* L, int index) {
    Vectormath::Aos::Vector3* vec = dmScript::CheckVector3(L, index);
    FMOD_VECTOR result = {
        .x = vec->getX(),
        .y = vec->getY(),
        .z = vec->getZ(),
    };
    return result;
}

#if defined(DM_PLATFORM_OSX)
#define CONFIG_SUFFIX "_macos"
#elif defined(DM_PLATFORM_WINDOWS)
#define CONFIG_SUFFIX "_windows"
#elif defined(DM_PLATFORM_LINUX)
#define CONFIG_SUFFIX "_linux"
#elif defined(DM_PLATFORM_IOS)
#define CONFIG_SUFFIX "_ios"
#elif defined(DM_PLATFORM_ANDROID)
#define CONFIG_SUFFIX "_android"
#elif defined(DM_PLATFORM_HTML5)
#define CONFIG_SUFFIX "_html5"
#elif defined(DM_PLATFORM_SWITCH)
#define CONFIG_SUFFIX "_switch"
#else
#define CONFIG_SUFFIX ""
#endif

const char* FMODExt_dmConfigFile_GetString(const char* config, const char* defaultValue) {
    const char* value = dmConfigFile::GetString(appConfig, config, defaultValue);

    size_t configLen = strlen(config);
    char* platformKey = new char[configLen + strlen(CONFIG_SUFFIX) + 1];
    strcpy(platformKey, config);
    strcpy(platformKey + configLen, CONFIG_SUFFIX);

    value = dmConfigFile::GetString(appConfig, platformKey, value);

    delete[] platformKey;
    return value;
}

int32_t FMODExt_dmConfigFile_GetInt(const char* config, int32_t defaultValue) {
    int32_t value = dmConfigFile::GetInt(appConfig, config, defaultValue);

    size_t configLen = strlen(config);
    char* platformKey = new char[configLen + strlen(CONFIG_SUFFIX) + 1];
    strcpy(platformKey, config);
    strcpy(platformKey + configLen, CONFIG_SUFFIX);

    value = dmConfigFile::GetInt(appConfig, platformKey, value);

    delete[] platformKey;
    return value;
}

#else

dmExtension::Result InitializeDefoldFMOD(dmExtension::Params* params) {
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeDefoldFMOD(dmExtension::Params* params) {
    return dmExtension::RESULT_OK;
}

#define UpdateDefoldFMOD 0
#define AppInitializeDefoldFMOD 0
#define OnEventDefoldFMOD 0

#endif

DM_DECLARE_EXTENSION(DefoldFMOD, LIB_NAME, AppInitializeDefoldFMOD, 0, InitializeDefoldFMOD, UpdateDefoldFMOD,
                     OnEventDefoldFMOD, FinalizeDefoldFMOD)
