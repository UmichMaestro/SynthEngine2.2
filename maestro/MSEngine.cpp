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

unsigned int bufsize = SAMPLE_WINDOW*BUFFER_MSM_COUNT;

static int rtaudio_callback(void *outbuf, void *inbuf, unsigned int nFrames, double streamtime, RtAudioStreamStatus status, void *userdata) {
    MSEngine* self = (MSEngine*)userdata;
    self->synthesize((float*)outbuf, nFrames);
    //
//    MSEngine* self = (MSEngine*)userdata;
//    CallbackData *data = (CallbackData*)self->data;
//    int time = data->time;
//
//    int partials = data->msm->partials;
//
//    int fundamentalFreq = data->msm->frequency;
//    double phaseIncr = F * fundamentalFreq;
//
//    double phase = data->phase;
//
//    int sustainStart = data->msm->sustainStart;
//    int sustainFinish = data->msm->sustainFinish;
//
//    double currentGain = data->currentGain;
//    double targetGain = data->targetGain;
//    double gainIncr = (currentGain == targetGain) ? 0 : (targetGain-currentGain)/nFrames;
//
//    for (int t = 0; t < BUFFER_MSM_COUNT; t++) {
//        if (time+t < sustainStart || time+t > sustainFinish) {
//            double *amp0 = data->msm->amplitudeForTime(time+t);
//            double *amp1 = data->msm->amplitudeForTime(time+t+1);
//            if (amp0 == NULL || amp1 == NULL) {
//                //                std::cout << "Reached the end.\n";
//                self->soundStop();
//                return 1; // TODO::::
//            }
//
//            for (int s = 0; s < SAMPLE_WINDOW; s++) {
//                for (int i=0; i<partials; i++) {
//                    double a0 = amp0[i];
//                    double a1 = amp1[i];
//                    double amp = a0 + (a1-a0)*s/(double)SAMPLE_WINDOW; // amp interpolation
//                    amp *= currentGain;
//                    double out = amp * sin(phase*(i+1)) / 1000;
//                    int outIndex = t*SAMPLE_WINDOW + s;
//                    buf[outIndex*2] += out;
//                    buf[outIndex*2+1] += out;
//                }
//                phase += phaseIncr;
//                currentGain += gainIncr;
//            }
//
//            data->time ++;
//        } else {
//            double *amp0 = data->msm->amplitudeForTime(sustainStart);
//            double *amp1 = data->msm->amplitudeForTime(sustainFinish);
//
//            for (int s = 0; s < SAMPLE_WINDOW; s++) {
//                for (int i=0; i<partials; i++) {
//                    double amp = (amp0[i] + amp1[i])/2.0;
//                    amp *= currentGain;
//                    double out = amp * sin(phase*(i+1)) / 1000;
//                    int outIndex = t*SAMPLE_WINDOW + s;
//                    buf[outIndex*2] += out;
//                    buf[outIndex*2+1] += out;
//                }
//                phase += phaseIncr;
//                currentGain += gainIncr;
//            }
//        }
//    }
//
//    data->phase = phase;
//    data->currentGain = targetGain;

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
    
    instruments = new vector<MSInstrument*>();
    
    audio->openStream(outParam, NULL, RTAUDIO_FLOAT32, SAMPLE_RATE, &bufsize, rtaudio_callback, this);
}

void MSEngine::synthesize(float *buf, unsigned int nFrames) {
    for (MSInstrument *i : *instruments) {
        i->synthesize(buf, nFrames);
    }
}

void MSEngine::attachInstrument(MSInstrument *inst) {
    instruments->push_back(inst);
}

void MSEngine::clearInstruments() {
    instruments->clear();
}
