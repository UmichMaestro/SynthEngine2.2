//
//  MSEngine.cpp
//  RtAudio
//
//  Created by Jungho Bang on 11/11/17.
//

#include "MSEngine.h"

static void errorCallback( RtAudioError::Type type, const std::string &errorText ) {
    std::cout << errorText;
}

unsigned int bufsize = SAMPLE_WINDOW*BUFFER_MSM_COUNT;

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
    
    instruments = new vector<MSInstNode*>();
    
    audio->openStream(outParam, NULL, RTAUDIO_FLOAT32, SAMPLE_RATE, &bufsize, staticCallback, this, NULL, errorCallback);
    audio->startStream();
}

vector<MSInstNode*>& MSEngine::getInstruments() {
    return *instruments;
}

void MSEngine::attachInstrument(MSInstNode *inst) {
    instruments->push_back(inst);
}

void MSEngine::clearInstruments() {
    instruments->clear();
}
