//
//  instrument.hpp
//  RtAudio
//
//  Created by Jungho Bang on 11/11/17.
//

#ifndef MSInstrument_h
#define MSInstrument_h

#include <stdio.h>
#include <string>

class MSInstNode {
    // msm
//    double frequency;
    double phaseIncr;
    uint32_t partials;
    uint32_t duration;
    uint32_t sustainStart;
    uint32_t sustainFinish;
    double *amplitudes;
    
    double phase;
    int time;
    double currentGain;
    double targetGain;
    
    double* amplitudeForTime(int t);
    void synthesize(float *outbuf, unsigned int nFrames);
    
public:
    MSInstNode(std::string path);
    
    void start(double initialGain = 1.0);
    void setGain(double gain);
    void release();
    
    friend class MSEngine;
};

#endif
