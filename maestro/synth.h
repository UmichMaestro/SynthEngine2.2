//
//  synth_2.1.h
//  Maestro Synth Engine
//
//  Created by Fisher Diede on 3/24/17
//  University of Michigan MDP
//

//call this function at the beginning of the program's execution to init the synth
void synthInit();

//call this function at the end of the program's execution to clean up the synth
void synthDestroy();

//call this to start a sound (call with no args for prototype)
void soundStart(int initialDynamic = 0, int articulation = 0, int duration = 0);

//call this to end a sound
void soundRelease();
