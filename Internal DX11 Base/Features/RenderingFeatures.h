#pragma once

namespace DX11Base {
    namespace Features {
        
        class RenderingFeatures {
        public:
            // Visual/Rendering features
            bool bighead = false;
            
            // Constructor
            RenderingFeatures();
            ~RenderingFeatures();
            
            // Feature Methods
            void InitializeFeatures();
            void UpdateFeatures();
            void ShutdownFeatures();
            
            // Individual Feature Controls
            void EnableBigHead();
            void DisableBigHead();
        };
        
        // Global instance
        inline std::unique_ptr<RenderingFeatures> g_RenderingFeatures;
    }
}
