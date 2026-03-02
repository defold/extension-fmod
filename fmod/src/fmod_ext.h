#ifndef FMOD_EXT_H
#define FMOD_EXT_H

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
#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)

#define FIRST(...) FIRST_HELPER(__VA_ARGS__, throwaway)
#define FIRST_HELPER(first, ...) first
#define REST(...) REST_HELPER(NUM(__VA_ARGS__), __VA_ARGS__)
#define REST_HELPER(qty, ...) REST_HELPER2(qty, __VA_ARGS__)
#define REST_HELPER2(qty, ...) REST_HELPER_##qty(__VA_ARGS__)
#define REST_HELPER_ONE(first)
#define REST_HELPER_TWOORMORE(first, ...) , __VA_ARGS__
#define NUM(...)                                                                                                     \
    SELECT_10TH(__VA_ARGS__, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, \
                ONE, throwaway)
#define SELECT_10TH(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) a10

#ifdef __cplusplus
extern "C" {
#endif

// Extension API
typedef unsigned int FMODExt_HBuffer;
int FMODExt_dmBuffer_GetBytes(FMODExt_HBuffer, void**, uint32_t*);
void FMODExt_dmScript_PushBuffer(lua_State* L, FMODExt_HBuffer);
FMODExt_HBuffer FMODExt_dmScript_CheckBuffer(lua_State* L, int);
void FMODExt_dmScript_PushVector3(lua_State* L, float x, float y, float z);
FMOD_VECTOR FMODExt_dmScript_CheckVector3(lua_State* L, int index);
const char* FMODExt_dmConfigFile_GetString(const char*, const char*);
int32_t FMODExt_dmConfigFile_GetInt(const char*, int32_t);

// Lifecycle functions
void FMODExt_init(lua_State* L);
void FMODExt_update();
void FMODExt_finalize();
void FMODExt_activateApp();
void FMODExt_deactivateApp();
void FMODExt_iconifyApp();
void FMODExt_deiconifyApp();

// Internal functions and members
extern FMOD_STUDIO_SYSTEM* FMODExt_system;
extern FMOD_SYSTEM* FMODExt_lowLevelSystem;
extern bool FMODExt_isPaused;

bool FMODExt_linkLibraries();
void FMODExt_cleanupLibraries();

void FMODExt_register(lua_State* L);
int FMODExt_getBundleRoot(lua_State* L);

void FMODExt_suspendMixer();
void FMODExt_resumeMixer();

#if TARGET_OS_IPHONE
void FMODExt_initIOSInterruptionHandler();
#endif

#define ensure(lib, fname, retType, ...)

#ifdef __cplusplus
}
#endif

#endif
