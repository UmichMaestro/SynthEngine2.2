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
#include "parse_msm.c"
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

typedef struct {
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
    
    /*
    
    //create buffers
    unsigned numBuffers = 4;
    ALuint buf[numBuffers];
    alGenBuffers(numBuffers, buf);
    al_check_error();
    float freq = 440.f;
    unsigned bufLen_ms = 20;
    unsigned sample_rate = 22050;
    size_t buf_size = double(bufLen_ms)/1000.0 * sample_rate;
    
    for (int i=0; i<numBuffers; ++i) {
        short *samples;
        samples = new short[buf_size];
        for(int s=0; s<buf_size; ++s) {
            samples[s] = 32765 * sin( (2.f*float(M_PI)*freq)/sample_rate * s );
        }
        alBufferData(buf[i], AL_FORMAT_MONO16, samples, buf_size, sample_rate);
        al_check_error();
    }
    
    */
    
    
    return 0;
}

int main(int argc, char* argv[]) {
    Model m = parseMSMAtPath("Horn.ff.C4B4-10-442.msm");
    
    std::cout << "0" << std::endl;
    RtAudio *audio;
    unsigned int bufsize = 4096;
    CallbackData *data = (CallbackData*)calloc(1, sizeof(CallbackData));
    try {
        audio = new RtAudio(RtAudio::MACOSX_CORE);
    }catch  (RtAudioError e){
        fprintf(stderr, "fail to create RtAudio: %s¥n", e.what());
        return 1;
    }
    if (!audio){
        fprintf(stderr, "fail to allocate RtAudio¥n");
        return 1;
    }
    /* probe audio devices */
    unsigned int devId = audio->getDefaultOutputDevice();
    std::cout << "1" << std::endl;
    /* Setup output stream parameters */
    RtAudio::StreamParameters *outParam = new RtAudio::StreamParameters();
    std::cout << "2" << std::endl;
    outParam->deviceId = devId;
    outParam->nChannels = 2;
    std::cout << "3" << std::endl;
    audio->openStream(outParam, NULL, RTAUDIO_FLOAT32, SAMPLE_RATE, &bufsize, rtaudio_callback, data);
    std::cout << "4" << std::endl;
    audio->startStream();
    std::cout << "5" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    audio->stopStream();
    audio->closeStream();
    delete audio;
    
    return 0;
}




/*
void alInit () {
    ALCdevice* Device = alcOpenDevice(alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));
    ALCcontext* Context = alcCreateContext(Device,NULL);
    alcMakeContextCurrent(Context);
}

void synthInit() {
    alInit();
}

void exit_al() {
    ALCdevice *dev = NULL;
    ALCcontext *ctx = NULL;
    ctx = alcGetCurrentContext();
    dev = alcGetContextsDevice(ctx);
    
    alcMakeContextCurrent(NULL);
    alcDestroyContext(ctx);
    alcCloseDevice(dev);
}
void synthDestroy() {
    t->join();
    exit_al();
}



void startSound(int initialDynamic = 0, int articulation = 0, int duration = 0) {
    stop = false;
    //make buffer to fill with sin
    ALuint buf;
    alGenBuffers(1, &buf);
    al_check_error();
    alBufferData(buf, AL_FORMAT_MONO16, inst.d, inst.numFrames*GRANULARITY, SAMPLE_RATE);
    al_check_error();
    
    
    //set up source
    ALuint src = 0;
    alGenSources(1, &src);
    alSourceQueueBuffers(src, 1, &buf);
    alSourcePlay(src);
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    
//    //wait until cutoff
//    sinLock.lock();
//    std::unique_lock<std::mutex> lock(sinLock, std::adopt_lock);
//    cutoff.wait(lock, []{return stop;});
//    
    //  turn off sin wave
    alSourceStop(src);
    
}

void soundStart(int initialDynamic, int articulation, int duration) {
    t = new std::thread(startSound, initialDynamic, articulation, duration);
    //sinThread->detach();
}

void releaseSound() {
    sinLock.lock();
    cutoff.notify_one();
    stop = true;
    sinLock.unlock();
}

void soundRelease() {
    std::thread* releaser = new std::thread(releaseSound);
    releaser->detach();
}
*/
