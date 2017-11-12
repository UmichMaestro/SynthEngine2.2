//
//  MSEngine.hpp
//  RtAudio
//
//  Created by Jungho Bang on 11/11/17.
//

#ifndef MSEngine_h
#define MSEngine_h

#include <string>
#include "RtAudio.h"
#include "MSInstrument.h"
#include "MSModel.h"

typedef struct {
    double phase;
    int time;
    MSModel *msm;
    double currentGain;
    double targetGain;
} CallbackData;

class MSEngine {
public:
    static MSEngine& sharedEngine() { // singleton
        static MSEngine instance;
        return instance;
    }
    
private:
    MSEngine();
    
    MSModel *msm;
    RtAudio *audio;
    RtAudio::StreamParameters *outParam;
    
public:
    CallbackData *data;
    
    void attach(MSInstrument *inst);
    std::vector<MSInstrument*> activeInstruments();
    void detach(MSInstrument *inst);
    
    // --------------
    
    void synthInit(std::string path);
    
    
    void soundStart(double initialGain = 1.0);
    void soundRelease();
    
    void updateGain(double targetGain);
    
    
    void soundStop();
    void synthDestroy();
};

#endif
