//
//  synth_2.1.cpp
//  Maestro Synth Engine
//
//  Created by Fisher Diede on 3/24/17
//  University of Michigan MDP
//

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "RtAudio.h"
/*
//globals
std::mutex sinLock;
std::condition_variable cutoff;
bool stop;
*/
double pi = 3.1415926535897;
float FREQUENCY = 440;
const float SAMPLE_RATE = 44100;
float SAMPLE_LEN = 1; //granularity of analysis data
int GRANULARITY = 220;

const double cyclesPerSample = FREQUENCY / SAMPLE_RATE; // [2]
const double angleDelta = cyclesPerSample * 2.0 * pi;

/*
typedef struct Model {
    double frequency;
    uint32_t partials;
    uint32_t duration;
    uint32_t loopStart; // attack - sustain
    uint32_t loopFinish; // sustain - release
    uint64_t empty[5]; // for later use...
} Model;


class Instrument_t {
private:
    double frequency;
public:
    std::vector<short> data;
    short *d;
    int numPartials;
    int numFrames;
    Instrument_t() {
        //read the model object
        Model msm;
        FILE *file = fopen("BbClar.mf.C5B5-9-819.msm","rb");
        if (!file) {
            printf("Unable to open file!");
        }
        fread(&msm, sizeof(Model), 1, file);
        numPartials = msm.partials;
        numFrames = msm.duration;
        frequency = msm.frequency;
        
        //pull amplitude matrix data from file
        int numCells = numPartials * numFrames;
        double * mat = new double[numCells];
        fread(mat, sizeof(double), numCells, file);
        fclose(file);
        //scale and store in array
        float amp[numFrames][numPartials];
        for (int row = 0; row < numFrames; ++row) {
            for (int col = 0; col < numPartials; ++col) {
                amp[row][col] = (float)((*mat)/64.0);
                ++mat;
            }
        }
        mat -= numCells;
        delete[] mat;
        
        //mix the signals
        d = new short[numFrames*GRANULARITY];
        
        for (int frame = 0; frame < numFrames; ++frame) {
            float buf[GRANULARITY];
            for (int i = 0; i < GRANULARITY; ++i) {
                buf[i] = 0.;
            }
            for (int part = 0; part < numPartials; ++part) {
                for (int i = 0; i < GRANULARITY; ++i) {
                    float a = buf[i];
                    float b = amp[frame][part] * sin( (2.f*float(M_PI)*(frequency*part))/SAMPLE_RATE * i);
                    buf[i] = (a+b) - (a*b);
                }
            }
            //scale to PCM level
            for (int i = 0; i < GRANULARITY; ++i) {
                *(d+(frame*GRANULARITY)+i) = buf[(frame*GRANULARITY)+i];
            }
        }
    }
    int getNumPartials() {
        return numPartials;
    }
    int getNumFrames() {
        return numFrames;
    }
    int getFrequency() {
        return frequency;
    }
};

Instrument_t inst;
std::thread *t;
*/

RtAudio *audio;

typedef struct CallbackData{
    double currentAngle;
} CallbackData;

static int rtaudio_callback(void *outbuf, void *inbuf, unsigned int nFrames, double streamtime, RtAudioStreamStatus status, void *userdata) {
    float *buf = (float*)outbuf;
    CallbackData *data = (CallbackData*)userdata;
    const float level = 0.125f;
    
    for (int sample = 0; sample < nFrames; ++sample)
    {
        float currentSample = (float) std::cos (data->currentAngle) * level;
        data->currentAngle += angleDelta;
        
        buf[sample*2] = currentSample;
        buf[sample*2+1] = currentSample;
    }
    
    return 0;
}

void synthInit() {
    unsigned int bufsize = 1024;
    CallbackData *data = (CallbackData*)calloc(1, sizeof(CallbackData));
    try {
        audio = new RtAudio();
    }catch  (RtAudioError e){
        fprintf(stderr, "fail to create RtAudio: %s¥n", e.what());
        //exit(1);
    }
    if (!audio){
        fprintf(stderr, "fail to allocate RtAudio¥n");
        //exit(1);
    }
    
    /* probe audio devices */
    unsigned int devId = audio->getDefaultOutputDevice();
    
    /* Setup output stream parameters */
    RtAudio::StreamParameters *outParam = new RtAudio::StreamParameters();
    outParam->deviceId = devId;
    outParam->nChannels = 2;
    audio->openStream(outParam, NULL, RTAUDIO_FLOAT32, SAMPLE_RATE, &bufsize, rtaudio_callback, data);
}

void synthDestroy() {
    audio->closeStream();
    delete audio;
}



void startSound(int initialDynamic = 0, int articulation = 0, int duration = 0) {
    //dB = 20 * log10(amplitude) then convert to scale of 0 - master
    audio->startStream();
}

void releaseSound() {
    audio->stopStream();
}


int main(int argc, char* argv[]) {
    synthInit();
    startSound();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    releaseSound();
    synthDestroy();
    return 0;
}
