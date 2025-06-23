#include "../pch.h"
#include "FeatureManager.h"

namespace DX11Base {
    namespace Features {
        
        // Global feature manager instance definition
        std::unique_ptr<FeatureManager> g_FeatureManager;
        
        FeatureManager::FeatureManager() {
            // Initialize all feature modules
            g_GameplayFeatures = std::make_unique<GameplayFeatures>();
            g_ZombieFeatures = std::make_unique<ZombieFeatures>();
            g_RenderingFeatures = std::make_unique<RenderingFeatures>();
        }
        
        FeatureManager::~FeatureManager() {
            ShutdownAllFeatures();
        }
        
        void FeatureManager::InitializeAllFeatures() {
            if (g_GameplayFeatures) {
                g_GameplayFeatures->InitializeFeatures();
            }
            
            if (g_ZombieFeatures) {
                g_ZombieFeatures->InitializeFeatures();
            }
            
            if (g_RenderingFeatures) {
                g_RenderingFeatures->InitializeFeatures();
            }
        }
        
        void FeatureManager::UpdateAllFeatures() {
            if (g_GameplayFeatures) {
                g_GameplayFeatures->UpdateFeatures();
            }
            
            if (g_ZombieFeatures) {
                g_ZombieFeatures->UpdateFeatures();
            }
            
            if (g_RenderingFeatures) {
                g_RenderingFeatures->UpdateFeatures();
            }
        }
        
        void FeatureManager::ShutdownAllFeatures() {
            if (g_GameplayFeatures) {
                g_GameplayFeatures.release();
            }
            
            if (g_ZombieFeatures) {
                g_ZombieFeatures.release();
            }
            
            if (g_RenderingFeatures) {
                g_RenderingFeatures.release();
            }
        }
    }
}
