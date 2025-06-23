#include "pch.h"
#include "IL2CPPHelper.h"
// This is the ONLY file that should include BNM headers directly
#include "JQ-BNM-main/BNM-IL2CPP/BNM.hpp"
#include <Windows.h>

namespace IL2CPPHelper
{
    // Global variables - definitions
    std::unordered_map<std::string, IL2CPP::Class*> cachedClasses;
    IL2CPP::Domain* domain = nullptr;
    bool initialized = false;
    std::unordered_map<uint64_t, std::vector<uint8_t>> originalBytesMap;
    std::mutex patchMutex;

    void DomainInitialized()
    {
        if (!initialized)
        {
            IL2CPP::Initialize(); // Initialize IL2CPP first
            domain = new IL2CPP::Domain();
            initialized = true;
        }
    }

    IL2CPP::Class* GetClass(const std::string& className)
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
                    std::size_t dotPos = className.find_last_of('.');
                    if (dotPos != std::string::npos)
                    {
                        std::string namespaceName = className.substr(0, dotPos);
                        std::string simpleName = className.substr(dotPos + 1);
                        
                        IL2CPP::Class* cls = image->Class(simpleName.c_str(), namespaceName.c_str());
                        if (cls)
                        {
                            cachedClasses[className] = cls;
                            return cls;
                        }
                    }
                    else
                    {
                        IL2CPP::Class* cls = image->Class(className.c_str());
                        if (cls)
                        {
                            cachedClasses[className] = cls;
                            return cls;
                        }
                    }
                }
            }
        }

        return nullptr;
    }

    IL2CPP::Object* CreateInstance(const std::string& className)
    {
        IL2CPP::Class* cls = GetClass(className);
        if (cls)
        {
            return cls->New();
        }
        return nullptr;
    }

    Unity::Vector3 WorldToScreen(Unity::Vector3 worldPos)
    {
        // Get the main camera
        IL2CPP::Class* cameraClass = GetClass("UnityEngine.Camera");
        if (!cameraClass) return Unity::Vector3();

        IL2CPP::Method mainMethod = cameraClass->Method("get_main", 0);
        if (!mainMethod.instance) return Unity::Vector3();

        IL2CPP::Object* mainCamera = mainMethod.Invoke<IL2CPP::Object*>();
        if (!mainCamera) return Unity::Vector3();

        // Call WorldToScreenPoint
        IL2CPP::Method worldToScreenMethod = cameraClass->Method("WorldToScreenPoint", 1);
        if (!worldToScreenMethod.instance) return Unity::Vector3();

        Unity::Vector3 screenPos = worldToScreenMethod.Invoke<Unity::Vector3>(mainCamera, worldPos);
        return screenPos;
    }

    std::vector<IL2CPP::Object*> FindObjectsOfType(const std::string& className)
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

    bool PatchBytes(void* address, const std::vector<uint8_t>& bytes)
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

        DWORD dummy;
        VirtualProtect(address, bytes.size(), oldProtect, &dummy);
        return true;
    }

    bool RestoreBytes(void* address)
    {
        std::lock_guard<std::mutex> lock(patchMutex);

        uint64_t addr = reinterpret_cast<uint64_t>(address);
        auto it = originalBytesMap.find(addr);
        if (it == originalBytesMap.end())
            return false;

        const std::vector<uint8_t>& original = it->second;
        
        DWORD oldProtect;
        if (!VirtualProtect(address, original.size(), PAGE_EXECUTE_READWRITE, &oldProtect))
            return false;

        memcpy(address, original.data(), original.size());

        DWORD dummy;
        VirtualProtect(address, original.size(), oldProtect, &dummy);
        
        originalBytesMap.erase(it);
        return true;
    }

    void DumpIL2CPPInfo(const std::string& outputDir)
    {
        DomainInitialized();
        
        _mkdir(outputDir.c_str()); // Use Windows API instead of std::filesystem
        
        std::ofstream outFile(outputDir + "\\IL2CPP_Dump.txt");
        if (!outFile.is_open()) return;

        outFile << "IL2CPP Information Dump\n";
        outFile << "========================\n\n";

        auto assemblies = domain->Assemblies();
        for (IL2CPP::Assembly* assembly : assemblies)
        {
            if (!assembly) continue;
            
            IL2CPP::Image* image = assembly->Image();
            if (!image) continue;

            outFile << "Assembly: " << image->Name() << "\n";
            outFile << "Classes Count: " << image->ClassCount() << "\n";
            outFile << "----------------------------------------\n";

            auto classes = image->Classes();
            for (IL2CPP::Class* cls : classes)
            {
                if (!cls) continue;
                
                outFile << "  Class: " << cls->Namespace() << "::" << cls->Name() << "\n";
                
                auto fields = cls->Fields();
                for (const auto& field : fields)
                {
                    outFile << "    Field: " << field.Name() << " (" << field.Type()->Name() << ")\n";
                }
                
                auto methods = cls->Methods();
                for (const auto& method : methods)
                {
                    outFile << "    Method: " << method.Name() << " (";
                    auto params = method.Parameters();
                    for (size_t i = 0; i < params.size(); i++)
                    {
                        if (i > 0) outFile << ", ";
                        outFile << params[i].ToString();
                    }
                    outFile << ") -> " << method.ReturnType()->Name() << "\n";
                }
                
                outFile << "\n";
            }
            outFile << "\n";
        }

        outFile.close();
    }

    uint64_t GetMethodAddress(const std::string& className, const std::string& methodName, int paramCount)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass) return 0;

        if (paramCount >= 0)
        {
            IL2CPP::Method method = klass->Method(methodName.c_str(), paramCount);
            if (method.instance)
            {
                return method.VA();
            }
        }
        else
        {
            // Find method by name only (first match)
            auto methods = klass->Methods();
            for (auto& method : methods)
            {
                if (strcmp(method.Name(), methodName.c_str()) == 0)
                {
                    return method.VA();
                }
            }
        }

        return 0;
    }

    void DumpClass(const std::string& className)
    {
        IL2CPP::Class* klass = GetClass(className);
        if (!klass)
        {
            printf("class %s not found!\n", className.c_str());
            return;
        }

        printf("=== Class: %s ===\n", className.c_str());
        printf("Namespace: %s\n", klass->Namespace());

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

    bool PatchMethod(const std::string& className, const std::string& methodName, const std::vector<uint8_t>& bytes)
    {
        uint64_t methodAddress = GetMethodAddress(className, methodName);
        if (!methodAddress) return false;

        return PatchBytes((void*)methodAddress, bytes);
    }

    bool NopMethod(const std::string& className, const std::string& methodName)
    {
        std::vector<uint8_t> retInstruction = { 0xC3 };
        return PatchMethod(className, methodName, retInstruction);
    }

    bool RestoreNopMethod(const std::string& className, const std::string& methodName)
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

    bool NopClassMethods(const std::string& className)
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
                    printf("can't NOP method: %s\n", method.Name());
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

    IL2CPP::Object* GetMainCamera()
    {
        IL2CPP::Class* camera = GetClass("UnityEngine.Camera");
        if (!camera)
        {
            return nullptr;
        }

        IL2CPP::Method meth = camera->Method("get_main");
        if (!meth.instance)
        {
            return nullptr;
        }

        return meth.Invoke<IL2CPP::Object*>();
    }

    IL2CPP::Class* GetClassPtr(const std::string& className)
    {
        return GetClass(className);
    }
}
