//
//  synth_2.1.h
//  Maestro Synth Engine
//
//  Created by Fisher Diede on 3/24/17
//  University of Michigan MDP
//
#include <string>
#include "RtAudio.h"
#include "MSM.h"

typedef struct {
    double phase;
    int time;
    MaestroSynthModel *msm;
    double currentGain;
    double targetGain;
} CallbackData;

class Synth {
    MaestroSynthModel *msm;
    RtAudio *audio;
    RtAudio::StreamParameters *outParam;
    
public:
    CallbackData *data;
    
//    Synth(std::string path);
    Synth();

    //call this function at the beginning of the program's execution to init the synth
    void synthInit(std::string path);
    
    //call this function at the end of the program's execution to clean up the synth
    void synthDestroy();
    
    //call this to start a sound (call with no args for prototype)
    void soundStart(double initialGain = 1.0);
    
    //call this to end a sound
    void soundRelease();
    
    void updateGain(double targetGain);
    
    void soundStop();
    
};

