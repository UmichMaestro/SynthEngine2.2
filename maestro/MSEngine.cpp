//
//  MSEngine.cpp
//  RtAudio
//
//  Created by Jungho Bang on 11/11/17.
//

//#include <cstdio>
//#include <cstdlib>
//#include <fstream>
//#include <iostream>
//#include <vector>
#include "MSEngine.h"
#include <cmath>

#define SAMPLE_RATE 44100.0
#define SAMPLE_WINDOW 441
#define BUFFER_MSM_COUNT 4 // how many msm amp data in each buffer
unsigned int bufsize = SAMPLE_WINDOW*BUFFER_MSM_COUNT;


static int rtaudio_callback(void *outbuf, void *inbuf, unsigned int nFrames, double streamtime, RtAudioStreamStatus status, void *userdata) {
    const double F = M_PI * 2 / SAMPLE_RATE;
    
    float *buf = (float*)outbuf;
    memset(outbuf, 0, nFrames*2*sizeof(float));
    
    MSEngine* self = (MSEngine*)userdata;
    CallbackData *data = (CallbackData*)self->data;
    int time = data->time;
    
    int partials = data->msm->partials;
    
    int fundamentalFreq = data->msm->frequency;
    double phaseIncr = F * fundamentalFreq;
    
    double phase = data->phase;
    
    int sustainStart = data->msm->sustainStart;
    int sustainFinish = data->msm->sustainFinish;
    
    double currentGain = data->currentGain;
    double targetGain = data->targetGain;
    double gainIncr = (currentGain == targetGain) ? 0 : (targetGain-currentGain)/nFrames;
    
    for (int t = 0; t < BUFFER_MSM_COUNT; t++) {
        if (time+t < sustainStart || time+t > sustainFinish) {
            double *amp0 = data->msm->amplitudeForTime(time+t);
            double *amp1 = data->msm->amplitudeForTime(time+t+1);
            if (amp0 == NULL || amp1 == NULL) {
                //                std::cout << "Reached the end.\n";
                self->soundStop();
                return 1; // TODO::::
            }
            
            for (int s = 0; s < SAMPLE_WINDOW; s++) {
                for (int i=0; i<partials; i++) {
                    double a0 = amp0[i];
                    double a1 = amp1[i];
                    double amp = a0 + (a1-a0)*s/(double)SAMPLE_WINDOW; // amp interpolation
                    amp *= currentGain;
                    double out = amp * sin(phase*(i+1)) / 1000;
                    int outIndex = t*SAMPLE_WINDOW + s;
                    buf[outIndex*2] += out;
                    buf[outIndex*2+1] += out;
                }
                phase += phaseIncr;
                currentGain += gainIncr;
            }
            
            data->time ++;
        } else {
            double *amp0 = data->msm->amplitudeForTime(sustainStart);
            double *amp1 = data->msm->amplitudeForTime(sustainFinish);
            
            for (int s = 0; s < SAMPLE_WINDOW; s++) {
                for (int i=0; i<partials; i++) {
                    double amp = (amp0[i] + amp1[i])/2.0;
                    amp *= currentGain;
                    double out = amp * sin(phase*(i+1)) / 1000;
                    int outIndex = t*SAMPLE_WINDOW + s;
                    buf[outIndex*2] += out;
                    buf[outIndex*2+1] += out;
                }
                phase += phaseIncr;
                currentGain += gainIncr;
            }
        }
    }
    
    data->phase = phase;
    data->currentGain = targetGain;
    
    return 0;
}

MSEngine::MSEngine() {
    try {
        audio = new RtAudio(RtAudio::MACOSX_CORE);
    }catch  (RtAudioError e){
        fprintf(stderr, "fail to create RtAudio: %s¥n", e.what());
        return ;
    }
    if (!audio){
        fprintf(stderr, "fail to allocate RtAudio¥n");
        return ;
    }
    /* probe audio devices */
    unsigned int devId = audio->getDefaultOutputDevice();
    /* Setup output stream parameters */
    outParam = new RtAudio::StreamParameters();
    outParam->deviceId = devId;
    outParam->nChannels = 2;
    
    data = new CallbackData();
}

void MSEngine::synthInit(std::string path) {
    msm = new MSModel(path);
    
    data->phase = 0;
    data->time = 0;
    data->msm = this->msm;
    
    //    std::cout << "Partials: " << msm->partials << std::endl;
    audio->openStream(outParam, NULL, RTAUDIO_FLOAT32, SAMPLE_RATE, &bufsize, rtaudio_callback, this);
    
    return;
}

void MSEngine::soundStart(double initialGain) {
    //    std::cout << "Start sound" << std::endl;
    data->currentGain = initialGain;
    data->targetGain = initialGain;
    
    audio->startStream();
}

void MSEngine::updateGain(double targetGain) {
    data->targetGain = targetGain;
}

void MSEngine::soundRelease() {
    //    std::cout << "Release" << std::endl;
    data->time = msm->sustainFinish;
}

void MSEngine::soundStop() {
    //    std::cout << "Stop" << std::endl;
    if(audio->isStreamOpen()) {
        audio->closeStream();
    }
}

void MSEngine::synthDestroy() {
    delete audio;
}
