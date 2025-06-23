#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include "../../Framework/imgui_internal.h"
#include <mutex>
#include <direct.h>
#include <io.h>
// Note: Removed std::filesystem dependency for compatibility

// Forward declarations to avoid including BNM headers in header file
namespace IL2CPP {
    struct Object;
    struct Class;
    struct Domain;
}

namespace Unity {
    struct Vector3 {
        float x, y, z;
        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    };
}

namespace IL2CPPHelper
{
    // Global variables - declarations only
    extern std::unordered_map<std::string, IL2CPP::Class*> cachedClasses;
    extern IL2CPP::Domain* domain;
    extern bool initialized;
    extern std::unordered_map<uint64_t, std::vector<uint8_t>> originalBytesMap;
    extern std::mutex patchMutex;

    // Function declarations
    void DomainInitialized();
    IL2CPP::Class* GetClass(const std::string& className);
    IL2CPP::Object* CreateInstance(const std::string& className);
    Unity::Vector3 WorldToScreen(Unity::Vector3 worldPos);
    std::vector<IL2CPP::Object*> FindObjectsOfType(const std::string& className);
    bool PatchBytes(void* address, const std::vector<uint8_t>& bytes);
    bool RestoreBytes(void* address);
    void DumpIL2CPPInfo(const std::string& outputDir);
    uint64_t GetMethodAddress(const std::string& className, const std::string& methodName, int paramCount = -1);
    void DumpClass(const std::string& className);
    bool PatchMethod(const std::string& className, const std::string& methodName, const std::vector<uint8_t>& bytes);
    bool NopMethod(const std::string& className, const std::string& methodName);
    bool RestoreNopMethod(const std::string& className, const std::string& methodName);
    bool NopClassMethods(const std::string& className);
    IL2CPP::Object* GetMainCamera();
    IL2CPP::Class* GetClassPtr(const std::string& className);    // Template functions must remain in header as inline
    template<typename T>
    inline bool SetMemberValue(const std::string& className, const std::string& fieldName, T value)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass) return false;

        IL2CPP::Field field = klass->Field(fieldName.c_str());
        if (!field.instance) return false;

        field.SetValue<T>(value);
        return true;
    }

    template<typename T>
    inline bool SetMemberValue(IL2CPP::Object* obj, const std::string& fieldName, T value)
    {
        if (!obj) return false;

        IL2CPP::Field field = obj->Field(fieldName.c_str());
        if (!field.instance) return false;

        field.SetValue<T>(value);
        return true;
    }

    template<typename T>
    inline T GetMemberValue(const std::string& className, const std::string& fieldName)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass) return T{};

        IL2CPP::Field field = klass->Field(fieldName.c_str());
        if (!field.instance) return T{};

        return field.GetValue<T>();
    }

    template<typename T>
    inline T GetMemberValue(IL2CPP::Object* obj, const std::string& fieldName)
    {
        if (!obj) return T{};

        IL2CPP::Field field = obj->Field(fieldName.c_str());
        if (!field.instance) return T{};

        return field.GetValue<T>(obj);
    }

    template<typename T, typename... Args>
    inline T CallMethod(const std::string& className, const std::string& methodName, Args... args)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass) return T{};

        IL2CPP::Method method = klass->Method(methodName.c_str(), sizeof...(args));
        if (!method.instance) return T{};

        return method.Invoke<T>(args...);
    }

    template<typename T, typename... Args>
    inline T CallMethod(IL2CPP::Object* obj, const std::string& methodName, Args... args)
    {
        if (!obj) return T{};

        IL2CPP::Method method = obj->Method(methodName.c_str(), sizeof...(args));
        if (!method.instance) return T{};

        return method.Invoke<T>(obj, args...);
    }

    template<typename RetType, typename... Args>
    inline RetType CallStaticMethod(const std::string& className, const std::string& methodName, Args... args)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass) return RetType{};

        IL2CPP::Method method = klass->Method(methodName.c_str());
        if (!method.instance) return RetType{};

        return method.Invoke<RetType>(args...);
    }
}
//useless macros
#define HookMethodC(className, methodName, hookFunc, origFuncPtr) \
    IL2CPPHelper::HookMethod(className, methodName, (void*)hookFunc, (void**)origFuncPtr)

#define createinstance(className) \
    IL2CPPHelper::CreateInstance(className)

#define dumpclass(className) \
    IL2CPPHelper::DumpClass(className)

#define nopmethod(className, methodName) \
    IL2CPPHelper::NopMethod(className, methodName)

#define nopclassmethods(className) \
    IL2CPPHelper::NopClassMethods(className)

#define HookC(className, methodName, hookFunc, origFuncPtr)                             \
    do {                                                                                      \
        uint64_t addr = IL2CPPHelper::GetMethodAddress(className, methodName);                                 \
        if (addr) {                                                                            \
            printf("%s::%s : 0x%llX\n", className, methodName, addr);                   \
            if (MH_CreateHook((void*)addr, hookFunc, (void**)origFuncPtr) == MH_OK) {         \
                if (MH_EnableHook((void*)addr) == MH_OK) {                                     \
                    printf("%s::%s hookd!\n", className, methodName);                   \
                } else {                                                                       \
                    printf("failed hooking %s::%s\n", className, methodName);       \
                }                                                                              \
            }                                                                       \
        } else {                                                                               \
            printf("method not found %s::%s\n", className, methodName);       \
        }                                                                                      \
    } while (0)
