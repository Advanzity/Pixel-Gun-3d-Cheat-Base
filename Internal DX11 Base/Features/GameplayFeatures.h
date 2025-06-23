#pragma once

namespace DX11Base {
    namespace Features {
        
        class GameplayFeatures {
        public:
            // Weapon Features
            bool MagicBullet = false;
            bool explosivebullet = false;
            bool infammo = false;
            bool shotgunmode = false;
            bool RapidFire = false;
            
            // Player Features
            bool doublejump = false;
            bool killpointshack = false;
            
            // Test Variables
            bool TestBool = false;
            int TestInt = 5;
            
            // Constructor
            GameplayFeatures();
            ~GameplayFeatures();
            
            // Feature Methods
            void InitializeFeatures();
            void UpdateFeatures();
            void ShutdownFeatures();
            
            // Individual Feature Controls
            void EnableMagicBullet();
            void DisableMagicBullet();
            void EnableExplosiveBullet();
            void DisableExplosiveBullet();
            void EnableInfiniteAmmo();
            void DisableInfiniteAmmo();
            void EnableShotgunMode();
            void DisableShotgunMode();
            void EnableRapidFire();
            void DisableRapidFire();
            void EnableDoubleJump();
            void DisableDoubleJump();
            void EnableKillPointsHack();
            void DisableKillPointsHack();
        };
        
        // Global instance - declaration only
        extern std::unique_ptr<GameplayFeatures> g_GameplayFeatures;
    }
}
