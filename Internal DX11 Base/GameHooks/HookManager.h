#pragma once

namespace DX11Base {
    namespace GameHooks {
        
        class HookManager {
        public:
            static void InitializeAllHooks();
            static void ShutdownAllHooks();
            
        private:
            static bool hooksInitialized;
        };
        
        // Global hook manager instance - declaration only
        extern std::unique_ptr<HookManager> g_HookManager;
    }
}
