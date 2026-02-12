# Mission Lost Control

A split-screen multiplayer shooter for the Nintendo 64, built in C with custom tooling. This was a team project where I handled AI systems, audio implementation, and critical asset pipeline tooling.

## Key Contributions

__AI & Pathfinding__ - Implemented combat AI state machines in `src/ai/ai_player.c` with distance-based attack logic. Reworked player pathfinding to support NPC navigation and multiplayer AI opponents. The runtime uses O(1) node-based pathfinding via precomputed lookup tables.

__Skelatool64 Pathfinding Generator__ - Built the pathfinding export functionality in the asset pipeline tool (`src/Pathfinding.cpp`). Implemented path table generation, automated node extraction from level meshes, and integrated it into the level export pipeline. This moved expensive pathfinding calculations from the N64 runtime to the build process on PC.

__Audio System__ - Integrated sound effects throughout the game (`DeathSFX`, `Footstep`, `PunchImpact`, `JumpSFX`). Modified `src/scene/level_scene.c` to trigger audio events and balanced audio levels through multiple iterations.

__UI & Polish__ - Designed and implemented the credits window system. performed campaign balancing, difficulty tuning, and various bug fixes including memory optimization.

