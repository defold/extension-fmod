#ifndef FMOD_BRIDGE_H
#define FMOD_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <dmsdk/lua/lua.h>
#include <dmsdk/lua/lauxlib.h>

#ifdef __cplusplus
}
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../include/fmod_studio.h"
#include "../include/fmod.h"

#ifdef __APPLE__
    #include "TargetConditionals.h"
#endif

#ifdef _WIN32
    #include <Windows.h>
#endif

#ifdef __ANDROID__
#include <android/log.h>
#include <jni.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "fmod", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "fmod", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "fmod", __VA_ARGS__))
#elif defined(_MSC_VER)
#define LOGI(fmt, ...) ((void)printf("INFO:fmod: " fmt "\n", __VA_ARGS__))
#define LOGE(fmt, ...) ((void)printf("ERROR:fmod: " fmt "\n", __VA_ARGS__))
#define LOGW(fmt, ...) ((void)printf("WARNING:fmod: " fmt "\n", __VA_ARGS__))
#else
#define LOGI(...) ((void)printf("INFO:fmod: " FIRST(__VA_ARGS__) "\n" REST(__VA_ARGS__)))
#define LOGE(...) ((void)printf("ERROR:fmod: " FIRST(__VA_ARGS__) "\n" REST(__VA_ARGS__)))
#define LOGW(...) ((void)printf("WARNING:fmod: " FIRST(__VA_ARGS__) "\n" REST(__VA_ARGS__)))
#endif

#define STRINGIFY(x) #x
#define RESOLVE(x) x
#define CONCAT_(a, b) a ## b
#define CONCAT(a, b) CONCAT_(a, b)

#define FIRST(...) FIRST_HELPER(__VA_ARGS__, throwaway)
#define FIRST_HELPER(first, ...) first
#define REST(...) REST_HELPER(NUM(__VA_ARGS__), __VA_ARGS__)
#define REST_HELPER(qty, ...) REST_HELPER2(qty, __VA_ARGS__)
#define REST_HELPER2(qty, ...) REST_HELPER_##qty(__VA_ARGS__)
#define REST_HELPER_ONE(first)
#define REST_HELPER_TWOORMORE(first, ...) , __VA_ARGS__
#define NUM(...) \
    SELECT_10TH(__VA_ARGS__, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE,\
                TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, ONE, throwaway)
#define SELECT_10TH(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) a10

#ifdef __cplusplus
extern "C" {
#endif

// Extension API
typedef unsigned int FMODBridge_HBuffer;
int FMODBridge_dmBuffer_GetBytes(FMODBridge_HBuffer, void**, uint32_t*);
void FMODBridge_dmScript_PushBuffer(lua_State* L, FMODBridge_HBuffer);
FMODBridge_HBuffer FMODBridge_dmScript_CheckBuffer(lua_State* L, int);
void FMODBridge_dmScript_PushVector3(lua_State* L, float x, float y, float z);
FMOD_VECTOR FMODBridge_dmScript_CheckVector3(lua_State* L, int index);
const char* FMODBridge_dmConfigFile_GetString(const char*, const char*);
int32_t FMODBridge_dmConfigFile_GetInt(const char*, int32_t);

// Lifecycle functions
void FMODBridge_init(lua_State* L);
void FMODBridge_update();
void FMODBridge_finalize();
void FMODBridge_activateApp();
void FMODBridge_deactivateApp();
void FMODBridge_iconifyApp();
void FMODBridge_deiconifyApp();

// Internal functions and members
extern FMOD_STUDIO_SYSTEM* FMODBridge_system;
extern FMOD_SYSTEM* FMODBridge_lowLevelSystem;
extern bool FMODBridge_isPaused;

bool FMODBridge_linkLibraries();
void FMODBridge_cleanupLibraries();

void FMODBridge_register(lua_State *L);
int FMODBridge_getBundleRoot(lua_State *L);

void FMODBridge_suspendMixer();
void FMODBridge_resumeMixer();

#if TARGET_OS_IPHONE
void FMODBridge_initIOSInterruptionHandler();
#endif

#define ensure(lib, fname, retType, ...)

#ifdef __cplusplus
}
#endif

#endif
