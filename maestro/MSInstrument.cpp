//
//  instrument.cpp
//  RtAudio
//
//  Created by Jungho Bang on 11/11/17.
//

#include "MSInstrument.h"
#include "MSEngine.h"
#include <cmath>

MSInstrument::MSInstrument(MSModel *msm) {
    this->msm = msm;
}

MSInstrument::MSInstrument(std::string path) {
    MSModel *msm = new MSModel(path);
    this->msm = msm;
}

void MSInstrument::synthesize(float *buf, unsigned int nFrames) {
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
        if (time+t < sustainStart || time+t > sustainFinish) {
            double *amp0 = msm->amplitudeForTime(time+t);
            double *amp1 = msm->amplitudeForTime(time+t+1);
            if (amp0 == NULL || amp1 == NULL) {
                std::cout << "Reached the end.\n";
                time = -1;
                return;
            }
            
            for (int s = 0; s < SAMPLE_WINDOW; s++) {
                for (int i=0; i<partials; i++) {
                    double a0 = amp0[i];
                    double a1 = amp1[i];
                    double amp = a0 + (a1-a0)*s/(double)SAMPLE_WINDOW; // amp interpolation
                    amp *= gain;
                    double out = amp * sin(p*(i+1)) / 1000;
                    int outIndex = t*SAMPLE_WINDOW + s;
                    buf[outIndex*2] += out;
                    buf[outIndex*2+1] += out;
                }
                p += phaseIncr;
                gain += gainIncr;
            }
            
            time ++;
        } else {
            double *amp0 = msm->amplitudeForTime(sustainStart);
            double *amp1 = msm->amplitudeForTime(sustainFinish);
            
            for (int s = 0; s < SAMPLE_WINDOW; s++) {
                for (int i=0; i<partials; i++) {
                    double amp = (amp0[i] + amp1[i])/2.0;
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
    }
    
    phase = p;
    currentGain = targetGain;
    
    return;
}

void MSInstrument::start(double initialGain) {
    currentGain = initialGain;
    targetGain = initialGain;
    
    time = 0;
}

void MSInstrument::setGain(double gain) {
    targetGain = gain;
}

void MSInstrument::release() {
    time = msm->sustainFinish;
}
