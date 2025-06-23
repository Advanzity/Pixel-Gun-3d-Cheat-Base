# Pixel Gun 3D Cheat Base - Improved Code Structure

## Overview
This project has been reorganized to improve code maintainability and organization by separating features into their own dedicated files and modules.

## New Directory Structure

```
Internal DX11 Base/
├── Core/                           # Core engine functionality
│   ├── D3D11Window.h              # DirectX 11 window and rendering management
│   ├── D3D11Window.cpp
│   ├── Hooking.h                  # Memory hooking functionality  
│   └── Hooking.cpp
├── Features/                       # Game-specific features organized by category
│   ├── FeatureManager.h           # Central feature management
│   ├── FeatureManager.cpp
│   ├── GameplayFeatures.h         # Gameplay-related cheats and features
│   ├── GameplayFeatures.cpp
│   ├── ZombieFeatures.h           # Zombie mode specific features
│   ├── ZombieFeatures.cpp
│   ├── RenderingFeatures.h        # Visual/rendering related features
│   └── RenderingFeatures.cpp
├── GameHooks/                      # Game-specific IL2CPP hooks and patches
│   ├── HookManager.h              # Central hook management
│   ├── HookManager.cpp
│   ├── WeaponHooks.h              # Weapon system hooks
│   ├── WeaponHooks.cpp
│   └── IL2CPP/                    # IL2CPP helper utilities
│       ├── IL2CPPHelper.h         # IL2CPP interaction utilities
│       └── JQ-BNM-main/           # IL2CPP framework (BNM)
├── Engine.h                       # Main engine class (simplified)
├── Engine.cpp
├── Menu.h                         # UI menu system
├── Menu.cpp
└── ... (other existing files)
```

## Key Improvements

### 1. **Modular Feature Organization**
- **GameplayFeatures**: Contains weapon features (Magic Bullet, Explosive Bullet, Infinite Ammo, etc.) and player features (Double Jump, Kill Points Hack)
- **ZombieFeatures**: Zombie-specific functionality (Anti Zombie Spawn)
- **RenderingFeatures**: Visual modifications (Big Head mode)

### 2. **Core System Separation**
- **D3D11Window**: All DirectX 11 related functionality moved to Core namespace
- **Hooking**: Memory hooking and patching functionality isolated

### 3. **Centralized Feature Management**
- **FeatureManager**: Provides centralized initialization, update, and shutdown of all features
- Easy access to any feature category through the main Engine class

### 4. **Game Hook Integration**
- **GameHooks**: Contains all IL2CPP hooks that apply features to the actual game
- **WeaponHooks**: Specifically hooks weapon systems to apply gameplay features
- **HookManager**: Centralized management of all game hooks
- **IL2CPPHelper**: Utilities for interacting with IL2CPP game objects

## Usage

### Accessing Features
```cpp
// Access gameplay features
auto* gameplayFeatures = g_Engine->GetFeatureManager()->GetGameplayFeatures();
gameplayFeatures->EnableMagicBullet();

// Access zombie features  
auto* zombieFeatures = g_Engine->GetFeatureManager()->GetZombieFeatures();
zombieFeatures->EnableAntiZombieSpawn();

// Access rendering features
auto* renderingFeatures = g_Engine->GetFeatureManager()->GetRenderingFeatures();
renderingFeatures->EnableBigHead();

// Access game hooks
auto* hookManager = g_Engine->GetHookManager();
// Hooks are automatically managed, but you can control them if needed
```

### Adding New Features
1. **Choose the appropriate feature category** (Gameplay, Zombie, or Rendering)
2. **Add the feature variable** to the corresponding header file
3. **Implement enable/disable methods** in the corresponding cpp file
4. **Update the menu** in Menu.cpp to include the new feature

### Creating New Feature Categories
1. Create new header/cpp files in the Features/ directory
2. Follow the same pattern as existing feature classes
3. Add the new feature class to FeatureManager
4. Update Engine.h to provide access if needed

### Adding New Game Hooks
1. Create new hook files in the GameHooks/ directory
2. Follow the pattern established by WeaponHooks
3. Add initialization/shutdown calls to HookManager
4. Make sure hooks reference the organized feature system

### MainPoint Folder Migration
The original `MainPoint/` folder has been reorganized as follows:
- `MainPoint/Hooks.h` → `GameHooks/WeaponHooks.h` and `GameHooks/WeaponHooks.cpp`
- `MainPoint/IL2CPPHelper.h` → `GameHooks/IL2CPP/IL2CPPHelper.h`
- `MainPoint/JQ-BNM-main/` → `GameHooks/IL2CPP/JQ-BNM-main/`

The functionality remains the same, but now it's properly organized and integrated with the new feature system.

## Benefits of This Structure

1. **Better Organization**: Features are logically grouped and easy to find
2. **Maintainability**: Each feature can be developed and tested independently
3. **Scalability**: Easy to add new features or entire feature categories
4. **Clean Separation**: Core engine functionality is separate from game features
5. **Namespace Protection**: Reduces naming conflicts and improves code clarity

## Migration Notes

- All feature variables have been moved from the main Engine class to their respective feature classes
- Menu.cpp has been updated to reference the new feature structure
- Core DirectX and hooking functionality now lives in the Core namespace
- Game hooks from MainPoint/ have been reorganized into GameHooks/ with proper separation
- IL2CPP functionality is now properly organized under GameHooks/IL2CPP/
- The main Engine class is now much cleaner and focused on coordination rather than feature storage
- WeaponHooks now references the organized feature system instead of direct Engine variables

## Future Enhancements

- Consider adding configuration saving/loading for features
- Implement feature dependency management
- Add logging and debugging capabilities for each feature category
- Create automated testing for individual features
- Expand GameHooks to include more game systems (player movement, inventory, etc.)
- Add hook conflict detection and resolution
- Implement runtime hook enable/disable functionality
