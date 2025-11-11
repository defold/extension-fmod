#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "fmod_bridge.h"
#ifdef FMOD_BRIDGE_LOAD_DYNAMICALLY

#ifndef _WIN32
#include <dlfcn.h>
#include <libgen.h>
#else
#include <Shlwapi.h>
#endif

#include <string.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#ifdef __linux__
#include <unistd.h>
#include <linux/limits.h>
#endif

dlModuleT FMODBridge_dlHandleLL = NULL;
dlModuleT FMODBridge_dlHandleST = NULL;

#if defined(_WIN32)
    #define SEP "\\"
    #define SEPCH '\\'
#else
    #define SEP "/"
    #define SEPCH '/'
#endif

#ifdef _WIN32
static char * dirname(char * path) {
    size_t i = strlen(path);
    do {
        i -= 1;
        if (path[i] == SEPCH) {
            path[i] = 0;
            break;
        }
    } while (i);
    return path;
}
#endif

#ifdef __ANDROID__

static JNIEnv* FMODBridge_jni_env = NULL;
static int FMODBridge_jni_refcount = 0;

void FMODBridge_attachJNI()
{
    if (FMODBridge_jni_refcount == 0) {
        JavaVM* vm = FMODBridge_dmGraphics_GetNativeAndroidJavaVM();
        (*vm)->AttachCurrentThread(vm, &FMODBridge_jni_env, NULL);
    }
    FMODBridge_jni_refcount += 1;
}

void FMODBridge_detachJNI()
{
    FMODBridge_jni_refcount -= 1;
    if (FMODBridge_jni_refcount == 0) {
        JNIEnv* env = FMODBridge_jni_env;
        bool exception = (bool)(*env)->ExceptionCheck(env);
        (*env)->ExceptionClear(env);
        JavaVM* vm = FMODBridge_dmGraphics_GetNativeAndroidJavaVM();
        (*vm)->DetachCurrentThread(vm);
    }
}

static jclass jniGetClass(JNIEnv* env, const char* classname) {
    jclass activity_class = (*env)->FindClass(env, "android/app/NativeActivity");
    jmethodID get_class_loader = (*env)->GetMethodID(env, activity_class,"getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject cls = (*env)->CallObjectMethod(env, FMODBridge_dmGraphics_GetNativeAndroidActivity(), get_class_loader);
    jclass class_loader = (*env)->FindClass(env, "java/lang/ClassLoader");
    jmethodID find_class = (*env)->GetMethodID(env, class_loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    jstring str_class_name = (*env)->NewStringUTF(env, classname);
    jclass outcls = (jclass)(*env)->CallObjectMethod(env, cls, find_class, str_class_name);
    (*env)->DeleteLocalRef(env, str_class_name);
    return outcls;
}

static void jniLogException(JNIEnv* env) {
    jthrowable e = (*env)->ExceptionOccurred(env);
    (*env)->ExceptionClear(env);

    jclass clazz = (*env)->GetObjectClass(env, e);
    jmethodID getMessage = (*env)->GetMethodID(env, clazz, "getMessage", "()Ljava/lang/String;");
    jstring message = (jstring)(*env)->CallObjectMethod(env, e, getMessage);
    const char *mstr = (*env)->GetStringUTFChars(env, message, NULL);
    LOGE("%s", mstr);
    (*env)->ReleaseStringUTFChars(env, message, mstr);
    (*env)->DeleteLocalRef(env, message);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, e);
}

void FMODBridge_cleanupLibraries() {
    attachJNI();
    JNIEnv* env = FMODBridge_jni_env;

    jclass fmodClass = jniGetClass(env, "org.fmod.FMOD");
    jmethodID closeMethod = (*env)->GetStaticMethodID(env, fmodClass, "close", "()V");
    (*env)->CallStaticVoidMethod(env, fmodClass, closeMethod);

    if ((*env)->ExceptionCheck(env)) {
        jniLogException(env);
    }

    detachJNI();
}

#else

static bool endsIn(const char * haystack, const char * needle) {
    size_t needleLen = strlen(needle);
    size_t haystackLen = strlen(haystack);
    return (haystackLen >= needleLen && 0 == strcmp(needle, haystack + haystackLen - needleLen));
}

void FMODBridge_cleanupLibraries() {
}
#endif
#endif
