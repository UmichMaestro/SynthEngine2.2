//
//  instrument.hpp
//  RtAudio
//
//  Created by Jungho Bang on 11/11/17.
//

#ifndef MSInstrument_h
#define MSInstrument_h

#include <stdio.h>
#include "MSModel.h"

class MSInstNode {
    MSModel *msm;
    double phase;
    int time;
    double currentGain;
    double targetGain;
    
    void synthesize(float *outbuf, unsigned int nFrames);
    
public:
    MSInstNode(MSModel *msm);
    MSInstNode(std::string path);
    
    void start(double initialGain = 1.0);
    void setGain(double gain);
    void release();
    
    friend class MSEngine;
};

#endif
