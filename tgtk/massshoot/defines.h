#pragma once

// Macros for clarity

#define AUDIO_FUNC void // Functions related to audio
#define LOADING_FUNC void // Functions that load or otherwise initialize elements
#define RENDERING_FUNC void // Functions related to rendering
#define INPUT_FUNC void // Functions that read inputs from the user
#define VITAL_FUNC void // If all goes well, you don't have to worry about them.
#define UI_FUNC void // Functions related to interfaces
#define PHYSICS_BOOL bool // Boolean functions related to physics
#define PHYSICS_FUNC void // Functions related to physics
#define LIGHTING_FUNC void // Functions related to lighting
#define SCRIPTING_BOOL bool // Boolean functions related to scripting
#define SCRIPTING_FUNC void // Functions related to scripting

extern int AmmoSetter;
extern int AmmoGetter;

#define GAMENAME "Dead Weather"