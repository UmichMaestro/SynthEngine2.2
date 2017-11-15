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

#define SAMPLE_RATE 44100.0
#define SAMPLE_WINDOW 441
#define BUFFER_MSM_COUNT 4 // how many msm amp data in each buffer

typedef struct {
    double phase;
    int time;
    MSModel *msm;
    double currentGain;
    double targetGain;
} CallbackData;

using namespace std;

class MSEngine {
public:
    static MSEngine& sharedEngine() { // singleton
        static MSEngine instance;
        return instance;
    }
    
private:
    MSEngine();
    RtAudio *audio;
    RtAudio::StreamParameters *outParam;
    vector<MSInstrument*> *instruments;
    
public:
    void synthesize(float *buf, unsigned int nFrames);
    
    void attachInstrument(MSInstrument *inst);
    void clearInstruments();
};

#endif
