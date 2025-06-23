#pragma once
#include "GameplayFeatures.h"
#include "ZombieFeatures.h"
#include "RenderingFeatures.h"

namespace DX11Base {
    namespace Features {
        
        class FeatureManager {
        public:
            FeatureManager();
            ~FeatureManager();
            
            // Manager Methods
            void InitializeAllFeatures();
            void UpdateAllFeatures();
            void ShutdownAllFeatures();
            
            // Feature Access
            GameplayFeatures* GetGameplayFeatures() { return g_GameplayFeatures.get(); }
            ZombieFeatures* GetZombieFeatures() { return g_ZombieFeatures.get(); }
            RenderingFeatures* GetRenderingFeatures() { return g_RenderingFeatures.get(); }
        };
        
        // Global feature manager instance - declaration only
        extern std::unique_ptr<FeatureManager> g_FeatureManager;
    }
}
