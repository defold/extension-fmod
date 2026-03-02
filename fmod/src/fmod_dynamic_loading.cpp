#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "fmod_ext.h"

// =============================================================================
// Android JNI initialization (required regardless of linking mode)
// =============================================================================
#ifdef __ANDROID__

#include <jni.h>
#include <dmsdk/dlib/android.h>

static void jniLogException(JNIEnv* env) {
    jthrowable e = env->ExceptionOccurred();
    env->ExceptionClear();

    jclass clazz = env->GetObjectClass(e);
    jmethodID getMessage = env->GetMethodID(clazz, "getMessage", "()Ljava/lang/String;");
    jstring message = (jstring)env->CallObjectMethod(e, getMessage);
    const char* mstr = env->GetStringUTFChars(message, NULL);
    LOGE("%s", mstr);
    env->ReleaseStringUTFChars(message, mstr);
    env->DeleteLocalRef(message);
    env->DeleteLocalRef(clazz);
    env->DeleteLocalRef(e);
}

bool FMODExt_linkLibraries() {
    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();
    if (!env) return false;

    if (env->ExceptionCheck()) {
        jniLogException(env);
        return false;
    }

    jclass fmodClass = dmAndroid::LoadClass(env, "org.fmod.FMOD");
    jmethodID initMethod = env->GetStaticMethodID(fmodClass, "init", "(Landroid/content/Context;)V");
    if (env->ExceptionCheck()) {
        jniLogException(env);
        return false;
    }
    env->CallStaticVoidMethod(fmodClass, initMethod, thread.GetActivity()->clazz);

    if (env->ExceptionCheck()) {
        jniLogException(env);
        return false;
    }

    LOGI("FMOD Java library initialized successfully");
    return true;
}

void FMODExt_cleanupLibraries() {
    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();
    if (!env) return;

    jclass fmodClass = dmAndroid::LoadClass(env, "org.fmod.FMOD");
    jmethodID closeMethod = env->GetStaticMethodID(fmodClass, "close", "()V");
    env->CallStaticVoidMethod(fmodClass, closeMethod);

    if (env->ExceptionCheck()) { jniLogException(env); }
}

// =============================================================================
// All other platforms (Defold handles library loading via ext.manifest)
// =============================================================================
#else

// No-op for platforms with static linking
bool FMODExt_linkLibraries() {
    return true;
}

void FMODExt_cleanupLibraries() {}

#endif
