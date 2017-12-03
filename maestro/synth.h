//
//  Synth Wrapper for C#
//  Maestro Synth Engine
//
//  Created by Fisher Diede on 3/24/17
// 	Updated by Jungho Bang on 12/2/17.
//  University of Michigan MDP
//

#include <string>
using namespace std;

//----- INIT
// Init doesn't need to be called multiple times (or, not at all). Now we use a singleton synth engine instance.
// To clear instrument nodes in the synth singleton. 
extern "C" __declspec(dllexport) void synthInit();

// To load a MSM in the path and add it to the engine. 
extern "C" __declspec(dllexport) void synthAttachInstrument(string path);

// (Previous version) It loads hard-coded MSMs and attach them to the singleton synth engine instance.
extern "C" __declspec(dllexport) void synthInit_prev();


//----- Control
//call this to start a sound (call with no args for prototype)
extern "C" __declspec(dllexport) void synthSoundStart(int index, double initialGain);

extern "C" __declspec(dllexport) void synthUpdateGain(int index, double targetGain);

//call this to end a sound
extern "C" __declspec(dllexport) void synthSoundRelease(int index);
