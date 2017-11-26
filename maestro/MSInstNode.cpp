//
//  instrument.cpp
//  RtAudio
//
//  Created by Jungho Bang on 11/11/17.
//

#include "MSInstNode.h"
#include "MSEngine.h"
#include <cmath>

MSInstNode::MSInstNode(MSModel *msm) {
    this->msm = msm;
    time = -1;
}

MSInstNode::MSInstNode(std::string path) {
    MSModel *msm = new MSModel(path);
    this->msm = msm;
    time = -1;
}

double amplitudeInterpolate(double *amp0, double *amp1, int partial, int sample, bool mean) {
    if (sample < 0) {
        return (amp0[partial] + amp1[partial])/2.0;
    } else {
        double a0 = amp0[partial];
        double a1 = amp1[partial];
        double amp = a0 + (a1-a0)*sample/(double)SAMPLE_WINDOW; // amp interpolation
        return amp;
    }
}

void MSInstNode::synthesize(float *buf, unsigned int nFrames) {
    if (time < 0) // not activated
        return;
    
    const double F = M_PI * 2 / SAMPLE_RATE;
    int partials = msm->partials;
    int fundamentalFreq = msm->frequency;
    double phaseIncr = F * fundamentalFreq;
    double p = phase;
    
    int sustainStart = msm->sustainStart;
    int sustainFinish = msm->sustainFinish;
    
    double gain = currentGain;
    double gainIncr = (gain == targetGain) ? 0 : (targetGain-gain)/nFrames;
    
    for (int t = 0; t < BUFFER_MSM_COUNT; t++) {
        bool mean;
        double *amp0, *amp1;
        
        if (time+t < sustainStart || time+t > sustainFinish) {
            mean = false;
            amp0 = msm->amplitudeForTime(time+t);
            amp1 = msm->amplitudeForTime(time+t+1);
            if (amp0 == NULL || amp1 == NULL) {
                std::cout << "Released" << endl;
                time = -1;
                return;
            }
            time ++;
        } else {
            mean = true;
            amp0 = msm->amplitudeForTime(sustainStart);
            amp1 = msm->amplitudeForTime(sustainFinish);
        }
            
        for (int s = 0; s < SAMPLE_WINDOW; s++) {
            for (int i=0; i<partials; i++) {
                double amp = amplitudeInterpolate(amp0, amp1, i, s, mean);
                amp *= gain;
                double out = amp * sin(p*(i+1)) / 1000;
                int outIndex = t*SAMPLE_WINDOW + s;
                buf[outIndex*2] += out;
                buf[outIndex*2+1] += out;
            }
            p += phaseIncr;
            gain += gainIncr;
        }
    }
    
    phase = p;
    currentGain = targetGain;
    
    return;
}

void MSInstNode::start(double initialGain) {
    std::cout << "Start" << endl;
    
    currentGain = initialGain;
    targetGain = initialGain;
    
    time = 0;
}

void MSInstNode::setGain(double gain) {
    targetGain = gain;
}

void MSInstNode::release() {
    std::cout << "Releasing..." << endl;
    
    time = msm->sustainFinish;
}
