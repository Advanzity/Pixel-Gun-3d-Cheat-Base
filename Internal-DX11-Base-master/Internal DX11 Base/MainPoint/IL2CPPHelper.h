#pragma once
#include <unordered_map>
#include "JQ-BNM-main/BNM-IL2CPP/Structures/IL2CPP.hpp"
#include "JQ-BNM-main/BNM-IL2CPP/Structures/UnityEngine.hpp"
#include <fstream>
#include <filesystem>
#include "../Framework/imgui_internal.h"
#include <mutex>
namespace fs = std::filesystem;

namespace IL2CPPHelper
{
    static std::unordered_map<std::string, IL2CPP::Class*> cachedClasses;
    static IL2CPP::Domain* domain = nullptr;
    static bool initialized = false;

    static void DomainInitialized()
    {
        if (!initialized)
        {
            domain = new IL2CPP::Domain();
            initialized = true;
        }
    }

    static IL2CPP::Class* GetClass(const std::string& className)
    {
        DomainInitialized();

        auto it = cachedClasses.find(className);
        if (it != cachedClasses.end())
            return it->second;

        std::vector<std::string> assemblies = {
            "Assembly-CSharp",
            "UnityEngine.CoreModule",
            "mscorlib",
            "System",
            "UnityEngine"
        };

        for (const auto& assemblyname : assemblies)
        {
            IL2CPP::Assembly* assembly = domain->Assembly(assemblyname.c_str());
            if (assembly)
            {
                IL2CPP::Image* image = assembly->Image();
                if (image)
                {
                    IL2CPP::Class* klass = image->Class(className.c_str());
                    if (klass)
                    {
                        cachedClasses[className] = klass;
                        return klass;
                    }
                }
            }
        }

        return nullptr;
    }





    template<typename T>
    static bool SetMemberValue(const std::string& className, const std::string& fieldName, T value)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass) return false;

        IL2CPP::Field field = klass->Field(fieldName.c_str());
        if (!field.instance) return false;

        field.SetValue<T>(value);
        return true;
    }

    template<typename T>
    static bool SetMemberValue(IL2CPP::Object* obj, const std::string& fieldName, T value)
    {
        if (!obj) return false;

        IL2CPP::Field field = obj->Field(fieldName.c_str());
        if (!field.instance) return false;

        field.SetValue<T>(value);
        return true;
    }

    template<typename T>
    static T GetMemberValue(const std::string& className, const std::string& fieldName)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass) return T{};

        IL2CPP::Field field = klass->Field(fieldName.c_str());
        if (!field.instance) return T{};

        return field.GetValue<T>();
    }

    template<typename T>
    static T GetMemberValue(IL2CPP::Object* obj, const std::string& fieldName)
    {
        if (!obj) return T{};

        IL2CPP::Field field = obj->Field(fieldName.c_str());
        if (!field.instance) return T{};

        return field.GetValue<T>();
    }

    static uint64_t GetMethodAddress(const std::string& className, const std::string& methodName, int paramCount = -1)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass) return 0;

        IL2CPP::Method method = klass->Method(methodName.c_str(), paramCount);
        if (!method.instance) return 0;

        return method.VA();
    }

    template<typename RetType, typename... Args>
    static RetType CallStaticMethod(const std::string& className, const std::string& methodName, Args... args)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass) return RetType{};

        IL2CPP::Method method = klass->Method(methodName.c_str());
        if (!method.instance) return RetType{};

        return method.Invoke<RetType>(args...);
    }

    template<typename RetType, typename... Args>
    static RetType CallMethod(IL2CPP::Object* obj, const std::string& methodName, Args... args)
    {
        if (!obj) return RetType{};

        IL2CPP::Method method = obj->Method(methodName.c_str());
        if (!method.instance) return RetType{};

        return method.Invoke<RetType>(args...);
    }

    static IL2CPP::Object* CreateInstance(const std::string& className)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass) return nullptr;

        return klass->New();
    }
        
    static bool TryGetMethodBySignature(
        const std::string& className,
        const std::string& returnType,
        const std::vector<std::string>& paramTypes,
        IL2CPP::Method& outMethod)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass) return false;

        auto methods = klass->Methods();
        for (auto& method : methods)
        {
            if (method.ReturnType() && method.ReturnType()->Name() == returnType)
            {
                auto params = method.Parameters();
                if (params.size() != paramTypes.size()) continue;

                bool match = true;
                for (size_t i = 0; i < paramTypes.size(); ++i)
                {
                    if (!params[i].type || params[i].type->Name() != paramTypes[i])
                    {
                        match = false;
                        break;
                    }
                }

                if (match)
                {
                    outMethod = method;
                    return true;
                }
            }
        }

        return false;
    }

    static void DumpClass(const std::string& className)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass)
        {
            printf("class %s not found!\n", className.c_str());
            return;
        }

        printf("=== Class: %s ===\n", className.c_str());
        printf("Namespaec: %s\n", klass->Namespace());

        printf("\nFields:\n");
        auto fields = klass->Fields();
        for (auto& field : fields)
        {
            printf("  %s: %s (Offset: 0x%X)\n",
                field.Type()->Name(),
                field.Name(),
                field.Offset());
        }

        printf("\nMethods:\n");
        auto methods = klass->Methods();
        for (auto& method : methods)
        {
            printf("  %s %s(", method.ReturnType()->Name(), method.Name());
            auto params = method.Parameters();
            for (size_t i = 0; i < params.size(); i++)
            {
                printf("%s", params[i].type->Name());
                if (i < params.size() - 1) printf(", ");
            }
            printf(") - VA: 0x%llX\n", method.VA());
        }
    }

    static std::vector<IL2CPP::Object*> FindObjectsOfType(const std::string& className)
    {
        std::vector<IL2CPP::Object*> objects;

        try
        {
            IL2CPP::Class* objectClass = GetClass("UnityEngine.Object");
            if (objectClass)
            {
                IL2CPP::Class* targetClass = GetClass(className);
                if (targetClass)
                {
                    IL2CPP::Type* targetType = targetClass->Type();
                    if (targetType)
                    {
                        IL2CPP::Method findMethod = objectClass->Method("FindObjectsOfType", 1);
                        if (findMethod.instance)
                        {
                            IL2CPP::Array* result = findMethod.Invoke<IL2CPP::Array*>(targetType);
                            if (result)
                            {
                                int length = result->Length();
                                for (int i = 0; i < length; i++)
                                {
                                    IL2CPP::Object* obj = result->Get<IL2CPP::Object*>(i);
                                    if (obj) objects.push_back(obj);
                                }
                            }
                        }
                    }
                }
            }
        }
        catch (...)
        {
            printf("TYPE NOT FOUND: %s\n", className.c_str());
        }

        return objects;
    }

    static std::unordered_map<uint64_t, std::vector<uint8_t>> originalBytesMap;
    static std::mutex patchMutex;

    static bool PatchBytes(void* address, const std::vector<uint8_t>& bytes)
    {
        std::lock_guard<std::mutex> lock(patchMutex);

        uint64_t addr = reinterpret_cast<uint64_t>(address);
        if (originalBytesMap.find(addr) == originalBytesMap.end())
        {
            std::vector<uint8_t> original(bytes.size());
            memcpy(original.data(), address, bytes.size());
            originalBytesMap[addr] = original;
        }

        DWORD oldProtect;
        if (!VirtualProtect(address, bytes.size(), PAGE_EXECUTE_READWRITE, &oldProtect))
            return false;

        memcpy(address, bytes.data(), bytes.size());

        VirtualProtect(address, bytes.size(), oldProtect, &oldProtect);
        return true;
    }


    static bool PatchMethod(const std::string& className, const std::string& methodName, const std::vector<uint8_t>& bytes)
    {
        uint64_t methodAddress = GetMethodAddress(className, methodName);
        if (!methodAddress) return false;

        return PatchBytes((void*)methodAddress, bytes);
    }

    static bool NopMethod(const std::string& className, const std::string& methodName)
    {
        std::vector<uint8_t> retInstruction = { 0xC3 };
        return PatchMethod(className, methodName, retInstruction);
    }

    static bool RestoreNopMethod(const std::string& className, const std::string& methodName)
    {
        uint64_t methodAddress = GetMethodAddress(className, methodName);
        if (!methodAddress) return false;

        std::lock_guard<std::mutex> lock(patchMutex);

        auto it = originalBytesMap.find(methodAddress);
        if (it == originalBytesMap.end()) return false;

        const auto& original = it->second;
        DWORD oldProtect;
        if (!VirtualProtect((void*)methodAddress, original.size(), PAGE_EXECUTE_READWRITE, &oldProtect))
            return false;

        memcpy((void*)methodAddress, original.data(), original.size());

        VirtualProtect((void*)methodAddress, original.size(), oldProtect, &oldProtect);
        originalBytesMap.erase(it);

        printf("restored: %s::%s\n", className.c_str(), methodName.c_str());
        return true;
    }


    static bool NopClassMethods(const std::string& className)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass) {
            printf("class not found: %s\n", className.c_str());
            return false;
        }

        bool success = true;
        auto methods = klass->Methods();
        for (auto& method : methods)
        {
            uint64_t addr = method.VA();
            if (addr)
            {
                if (!PatchBytes((void*)addr, { 0xC3 }))
                {
                    printf("cnat NOP method: %s\n", method.Name());
                    success = false;
                }
                else
                {
                    printf("NOPd method: %s\n", method.Name());
                }
            }
        }

        return success;
    }

    static IL2CPP::Object* GetMainCamera()
    {
        IL2CPP::Class* camera = GetClass("UnityEngine.Camera");
        if (!camera)
        {
            return 0;
        }

        IL2CPP::Method meth = camera->Method("get_main");
        if (!meth.instance)
        {
            return 0;
        }

        return meth.Invoke<IL2CPP::Object*>();
    }

    Unity::Vector3 WorldToScreen(Unity::Vector3 worldPos)
    {
        IL2CPP::Object* camera = GetMainCamera();
        if (!camera)
        {
            return Unity::Vector3();
        }

        auto methodorfunc = camera->Method("WorldToScreenPoint", 1);
        if (!methodorfunc.instance)
        {
            return Unity::Vector3();
        }

        return methodorfunc.Invoke<Unity::Vector3>(worldPos);
    }


    static IL2CPP::Class* GetClassPtr(const std::string& className)
    {
        return GetClass(className);
    }

    static void DumpGme(const std::string& outputDir = "ShameDump")
    {
        DomainInitialized();

        fs::create_directories(outputDir);

        std::vector<std::string> assemblies = {
            "Assembly-CSharp",
            "UnityEngine.CoreModule",
            "mscorlib",
            "System",
            "UnityEngine"
        };

        for (const auto& assemblyName : assemblies)
        {
            IL2CPP::Assembly* assembly = domain->Assembly(assemblyName.c_str());
            if (!assembly) continue;

            IL2CPP::Image* image = assembly->Image();
            if (!image) continue;

            auto classes = image->Classes();
            for (auto* klass : classes)
            {
                if (!klass) continue;

                std::string className = klass->Name();
                std::string namespaceName = klass->Namespace();
                std::string fileName = outputDir + "/" + className + ".cs";

                std::ofstream outFile(fileName);
                if (!outFile.is_open()) continue;

                outFile << "// Decompiled with IL2CPPHelper by [7ddf aka 0SD47]\n"; //roblox type shi
                if (!namespaceName.empty())
                {
                    outFile << "namespace " << namespaceName << "\n{\n";
                    outFile << "    public class " << className << "\n    {\n";
                }
                else
                {
                    outFile << "public class " << className << "\n{\n";
                }

                auto fields = klass->Fields();
                for (auto& field : fields)
                {
                    outFile << "        public " << field.Type()->Name() << " " << field.Name() << ";\n";
                }

                auto methods = klass->Methods();
                for (auto& method : methods)
                {
                    outFile << "        public " << method.ReturnType()->Name() << " " << method.Name() << "(";
                    auto params = method.Parameters();
                    for (size_t i = 0; i < params.size(); i++)
                    {
                        outFile << params[i].type->Name() << " param" << i;
                        if (i < params.size() - 1) outFile << ", ";
                    }
                    outFile << ") { }\n";
                }

                if (!namespaceName.empty())
                {
                    outFile << "    }\n}\n";
                }
                else
                {
                    outFile << "}\n";
                }

                outFile.close();
            }
        }

        printf("Dumped! loc: %s\n", outputDir.c_str());
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
 