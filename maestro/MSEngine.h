//
//  MSEngine.hpp
//  RtAudio
//
//  Created by Jungho Bang on 11/11/17.
//

#include <string>
#include "RtAudio.h"
#include "MSModel.h"

typedef struct {
    double phase;
    int time;
    MSModel *msm;
    double currentGain;
    double targetGain;
} CallbackData;

class MSEngine {
    MSModel *msm;
    RtAudio *audio;
    RtAudio::StreamParameters *outParam;
    
public:
    CallbackData *data;
    
    //    Synth(std::string path);
    MSEngine();
    
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


