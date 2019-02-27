//
// Created by lpcdm on 2019/2/27.
//
#include <jni.h>
#include <whale.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <iostream>

#include <android/log.h>

#define MODULE_NAME  "LOGXX"
#define LOGD(...) __android_log_print(ANDROID_LOG_ERROR, MODULE_NAME, __VA_ARGS__)

char *(*Origin_getenv)(const char *);

char *Hooked_getenv(const char *name) {
    if (!strcmp(name, "lody")) {
        return strdup("are you ok?");
    }
    char *(*O)(const char *) = Origin_getenv;
    return O(name);
}


extern "C" JNIEXPORT void JNICALL
Java_com_example_nativehookdeme_NativeHookTest_toDo(
        JNIEnv *env,
        jclass type) {
#if defined(__APPLE__)
    void *handle = dlopen("libc.dylib", RTLD_NOW);
#else
    void *handle = dlopen("libc.so", RTLD_NOW);
#endif
    assert(handle != nullptr);
    void *symbol = dlsym(handle, "getenv");
    assert(symbol != nullptr);
    WInlineHookFunction(
            symbol,
            reinterpret_cast<void *>(Hooked_getenv),
            reinterpret_cast<void **>(&Origin_getenv)
    );
//    WImportHookFunction(
//            "_getenv",
//            reinterpret_cast<void *>(Hooked_getenv),
//            reinterpret_cast<void **>(&Origin_getenv)
//    );

    const char *val = getenv("lody");
    LOGD("out ==> %s", val);
}

