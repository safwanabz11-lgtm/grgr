#pragma once
#include <jni.h>
#include <sys/types.h>

namespace zygisk {

enum ApiVersion {
    V1 = 1,
    V2 = 2,
    V3 = 3,
    V4 = 4,
};

enum Option {
    DLCLOSE_MODULE_LIBRARY = 0,
};

class Api {
public:
    virtual int getApiVersion() = 0;
    virtual void setOption(Option opt) = 0;
    virtual int connectCompanion() = 0;
    virtual void pltHookRegister(dev_t dev, ino_t ino, const char *symbol, void *new_func, void **old_func) = 0;
    virtual bool pltHookCommit() = 0;
    virtual void *hookJniNativeMethods(JNIEnv *env, const char *className, const JNINativeMethod *methods, int numMethods) = 0;
    virtual void *zbiHookRegister(void *target, void *replacement) = 0;
};

struct AppSpecializeArgs {
    jint &uid;
    jint &gid;
    jintArray &gids;
    jint &runtimeFlags;
    jobjectArray &permData;
    jint &mountExternal;
    jstring &seInfo;
    jstring &niceName;
    jstring &instructionSet;
    jstring &appDataDir;
};

struct ServerSpecializeArgs {
    jint &uid;
    jint &gid;
    jintArray &gids;
    jint &runtimeFlags;
    jobjectArray &permData;
    jstring &seInfo;
    jstring &niceName;
};

class ModuleBase {
public:
    virtual void onLoad(Api *api, JNIEnv *env) {}
    virtual void preAppSpecialize(AppSpecializeArgs *args) {}
    virtual void postAppSpecialize(const AppSpecializeArgs *args) {}
    virtual void preServerSpecialize(ServerSpecializeArgs *args) {}
    virtual void postServerSpecialize(const ServerSpecializeArgs *args) {}
    virtual ~ModuleBase() {}
};

} // namespace zygisk

#define REGISTER_ZYGISK_MODULE(clazz) \
    static clazz __zygisk_module; \
    extern "C" __attribute__((visibility("default"))) \
    void zygisk_module_entry(zygisk::Api *api, JNIEnv *env) { \
        __zygisk_module.onLoad(api, env); \
    }

#define REGISTER_ZYGISK_COMPANION(handler) \
    extern "C" __attribute__((visibility("default"))) \
    void zygisk_companion_entry(int client) { \
        handler(client); \
    }
