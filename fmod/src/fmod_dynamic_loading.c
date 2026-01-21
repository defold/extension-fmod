#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "fmod_bridge.h"

// =============================================================================
// Android JNI initialization (required regardless of linking mode)
// =============================================================================
#ifdef __ANDROID__

#include <jni.h>

static JNIEnv* FMODBridge_jni_env = NULL;
static int FMODBridge_jni_refcount = 0;

void FMODBridge_attachJNI()
{
    if (FMODBridge_jni_refcount == 0) {
        JavaVM* vm = FMODBridge_dmGraphics_GetNativeAndroidJavaVM();
        vm->AttachCurrentThread(&FMODBridge_jni_env, NULL);
    }
    FMODBridge_jni_refcount += 1;
}

void FMODBridge_detachJNI()
{
    FMODBridge_jni_refcount -= 1;
    if (FMODBridge_jni_refcount == 0) {
        JNIEnv* env = FMODBridge_jni_env;
        env->ExceptionClear();
        JavaVM* vm = FMODBridge_dmGraphics_GetNativeAndroidJavaVM();
        vm->DetachCurrentThread();
    }
}

static jclass jniGetClass(JNIEnv* env, const char* classname) {
    jclass activity_class = env->FindClass("android/app/NativeActivity");
    jmethodID get_class_loader = env->GetMethodID(activity_class, "getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject cls = env->CallObjectMethod(FMODBridge_dmGraphics_GetNativeAndroidActivity(), get_class_loader);
    jclass class_loader = env->FindClass("java/lang/ClassLoader");
    jmethodID find_class = env->GetMethodID(class_loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    jstring str_class_name = env->NewStringUTF(classname);
    jclass outcls = (jclass)env->CallObjectMethod(cls, find_class, str_class_name);
    env->DeleteLocalRef(str_class_name);
    return outcls;
}

static void jniLogException(JNIEnv* env) {
    jthrowable e = env->ExceptionOccurred();
    env->ExceptionClear();

    jclass clazz = env->GetObjectClass(e);
    jmethodID getMessage = env->GetMethodID(clazz, "getMessage", "()Ljava/lang/String;");
    jstring message = (jstring)env->CallObjectMethod(e, getMessage);
    const char *mstr = env->GetStringUTFChars(message, NULL);
    LOGE("%s", mstr);
    env->ReleaseStringUTFChars(message, mstr);
    env->DeleteLocalRef(message);
    env->DeleteLocalRef(clazz);
    env->DeleteLocalRef(e);
}

// Android: Initialize FMOD Java library (required for both static and dynamic linking)
bool FMODBridge_linkLibraries() {
    attachJNI();
    JNIEnv* env = FMODBridge_jni_env;

    if (env->ExceptionCheck()) {
        jniLogException(env);
        detachJNI();
        return false;
    }

    // Initialize the FMOD Java lib - this loads libfmod.so and libfmodstudio.so
    jclass fmodClass = jniGetClass(env, "org.fmod.FMOD");
    jmethodID initMethod = env->GetStaticMethodID(fmodClass, "init", "(Landroid/content/Context;)V");
    if (env->ExceptionCheck()) {
        jniLogException(env);
        detachJNI();
        return false;
    }
    env->CallStaticVoidMethod(fmodClass, initMethod, FMODBridge_dmGraphics_GetNativeAndroidActivity());

    if (env->ExceptionCheck()) {
        jniLogException(env);
        detachJNI();
        return false;
    }

    LOGI("FMOD Java library initialized successfully");
    detachJNI();
    return true;
}

void FMODBridge_cleanupLibraries() {
    attachJNI();
    JNIEnv* env = FMODBridge_jni_env;

    jclass fmodClass = jniGetClass(env, "org.fmod.FMOD");
    jmethodID closeMethod = env->GetStaticMethodID(fmodClass, "close", "()V");
    env->CallStaticVoidMethod(fmodClass, closeMethod);

    if (env->ExceptionCheck()) {
        jniLogException(env);
    }

    detachJNI();
}

// =============================================================================
// All other platforms (Defold handles library loading via ext.manifest)
// =============================================================================
#else

// No-op for platforms with static linking
bool FMODBridge_linkLibraries() {
    return true;
}

void FMODBridge_cleanupLibraries() {
}

#endif
