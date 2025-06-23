#pragma once

namespace DX11Base {
    namespace Features {
        
        class ZombieFeatures {
        public:
            // Zombie-related features
            bool AntiZombieSpawn = false;
            
            // Constructor
            ZombieFeatures();
            ~ZombieFeatures();
            
            // Feature Methods
            void InitializeFeatures();
            void UpdateFeatures();
            void ShutdownFeatures();
            
            // Individual Feature Controls
            void EnableAntiZombieSpawn();
            void DisableAntiZombieSpawn();
        };
        
        // Global instance
        inline std::unique_ptr<ZombieFeatures> g_ZombieFeatures;
    }
}
