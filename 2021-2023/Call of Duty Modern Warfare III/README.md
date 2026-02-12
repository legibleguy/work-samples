* Call of Duty Modern Warfare III (2023)

I was a software engineering intern during the 2025 summer season. i was overlooking two projects in the Warzone 2.0:
1. A battle royale circle aka "the storm"
	* I developed the shader that visualized the inside and the outside of the storm on the map. outside of the battle royale storm would activate a specific shader + some post processing effects. the key function here was the SDF for a sphere:
		`float sdSphere( vec3 p, float s ) { return length(p)-s; }`
2. New UAV type
	* In collaboration with the game design team, I developed a new type of UAVs that would ping enemies who are currently swimming in the water. This was helpful for my studio's new map they were developing because one of the highlights in it was a gameplay section that happened on the boat that was circling the map.
