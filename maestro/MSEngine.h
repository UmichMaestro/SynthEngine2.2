//
//  MSEngine.hpp
//  RtAudio
//
//  Created by Jungho Bang on 11/11/17.
//

#ifndef MSEngine_h
#define MSEngine_h

#include <stdio.h>
#include "RtAudio.h"
#include "MSInstrument.h"
#include "MSModel.h"

#define SAMPLE_RATE 44100.0
#define SAMPLE_WINDOW 441
#define BUFFER_MSM_COUNT 4 // how many msm amp data in each buffer

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
    
    static int staticCallback(void *outbuf, void *inbuf, unsigned int nFrames, double streamtime, RtAudioStreamStatus status, void *userdata) {
        memset(outbuf, 0, nFrames*2*sizeof(float));
        for (MSInstrument *i : ((MSEngine*)userdata)->getInstruments())
            i->synthesize((float*)outbuf, nFrames);
        return 0;
    }
    
public:
    void attachInstrument(MSInstrument *inst);
    void clearInstruments();
    vector<MSInstrument*>& getInstruments();
};

#endif
