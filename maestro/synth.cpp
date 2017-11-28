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
#include <vector>
#include "synth.h"
#include <memory.h>


/*
//globals
std::mutex sinLock;
std::condition_variable cutoff;
bool stop;
*/
#define _USE_MATH_DEFINES

#ifndef M_PI
	#define M_PI 3.1415926358979323846
#endif


#define SAMPLE_RATE 44100.0
#define SAMPLE_WINDOW 441
#define BUFFER_MSM_COUNT 4 // how many msm amp data in each buffer
unsigned int bufsize = SAMPLE_WINDOW*BUFFER_MSM_COUNT;

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

static void error_callback(RtAudioError::Type type, const std::string &error) {
	std::cout << "This is error::::: " + error << std::endl;
}

	static int rtaudio_callback(void *outbuf, void *inbuf, unsigned int nFrames, double streamtime, RtAudioStreamStatus status, void *userdata) {
		const double F = M_PI * 2 / SAMPLE_RATE;

		float *buf = (float*)outbuf;
		memset(outbuf, 0, nFrames * 2 * sizeof(float));

		Synth* self = (Synth*)userdata;
		CallbackData *data = (CallbackData*)self->data;
		
		if (data->isPlaying == false)
			return 0;

		int time = data->time;

		int partials = data->msm->partials;

		int fundamentalFreq = data->msm->frequency;
		double phaseIncr = F * fundamentalFreq;

		double phase = data->phase;

		int sustainStart = data->msm->sustainStart;
		int sustainFinish = data->msm->sustainFinish;

		double currentGain = data->currentGain;
		double targetGain = data->targetGain;
		double gainIncr = (currentGain == targetGain) ? 0 : (targetGain - currentGain) / nFrames;
		
		for (int t = 0; t < BUFFER_MSM_COUNT; t++) {
			if (time + t < sustainStart || time + t > sustainFinish) {
				double *amp0 = data->msm->amplitudeForTime(time + t);
				double *amp1 = data->msm->amplitudeForTime(time + t + 1);
				if (amp0 == NULL || amp1 == NULL) {
					//                std::cout << "Reached the end.\n";
					//self->soundStop();
					data->isPlaying = false;
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

	Synth::Synth() {
		try {
			audio = new RtAudio(RtAudio::UNSPECIFIED);
		}
		catch (RtAudioError e) {
			fprintf(stderr, "fail to create RtAudio: %s¥n", e.what());
			return;
		}
		if (!audio) {
			fprintf(stderr, "fail to allocate RtAudio¥n");
			return;
		}
		/* probe audio devices */
		unsigned int devId = audio->getDefaultOutputDevice();
		/* Setup output stream parameters */
		outParam = new RtAudio::StreamParameters();
		outParam->deviceId = devId;
		outParam->nChannels = 2;

		data = new CallbackData();
	}


	void Synth::msmInit() {
		std::string paths[5] = { 
			"C:\\Users\\MDPMaestro2\\Desktop\\w17Changes\\MaestroRun\\maestro_run\\instruments\\BbClar.ff.C4B4-5-330.msm" ,
			"C:\\Users\\MDPMaestro2\\Desktop\\w17Changes\\MaestroRun\\maestro_run\\instruments\\Bassoon.mf.C3B3-1-131.msm",
			"C:\\Users\\MDPMaestro2\\Desktop\\w17Changes\\MaestroRun\\maestro_run\\instruments\\flute.nonvib.ff.B3B4-11-442.msm",
			"C:\\Users\\MDPMaestro2\\Desktop\\w17Changes\\MaestroRun\\maestro_run\\instruments\\Horn.ff.C4B4-10-442.msm",
			"C:\\Users\\MDPMaestro2\\Desktop\\w17Changes\\MaestroRun\\maestro_run\\instruments\\oboe.mf.C4B4-8-441.msm"
		};

		for (int i = 0; i < 5; ++i) {
			msms[i] = new MaestroSynthModel(paths[i]);

		}
		msm = msms[0];

	}

	void Synth::synthInit() {
		std::cout << "Am I reaching this point?" << std::endl;
		msmInit();
		data->phase = 0;
			data->time = 0;
			data->msm = this->msm;
			std::cout << "can i see this?" << std::endl;
			//    std::cout << "Partials: " << msm->partials << std::endl;
			audio->openStream(outParam, NULL, RTAUDIO_FLOAT32, SAMPLE_RATE, &bufsize, rtaudio_callback, this);// , 0, error_callback	);
		
		return;
	}

	void Synth::soundStart(int index, double initialGain) {
		    std::cout << "Start sound " << index << std::endl;
		msm = msms[index];
		data->msm = this->msm;
		data->phase = 0;
		data->time = 0;
		data->isPlaying = true;

		data->currentGain = initialGain;
		data->targetGain = initialGain;

		audio->startStream();
	}

	void Synth::updateGain(double targetGain) {
		data->targetGain = targetGain;
	}

	void Synth::soundRelease() {
		//    std::cout << "Release" << std::endl;
		data->time = msm->sustainFinish;
	}

	void Synth::soundStop() {
		//    std::cout << "Stop" << std::endl;
		if (audio->isStreamOpen()) {
			audio->closeStream();
		}
	}

	void Synth::synthDestroy() {
		delete audio;
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

	Synth engine;

	void synthInit_alias() {
		//engine.synthInit("C:\\Users\\MDPMaestro2\\Desktop\\w17Changes\\MaestroRun\\maestro_run\\oboe.mf.C6Ab6-7-706.msm");
		
		std::cout << "Before synth init call" << std::endl;
		engine.synthInit();
		

		
	}
		
	//call this function at the end of the program's execution to clean up the synth
	void synthDestroy_alias() {
		engine.synthDestroy();
	}

	//call this to start a sound (call with no args for prototype)
	void soundStart_alias(int index, double initialGain) {
		//try {
		//engine.synthInit("C:\\Users\\MDPMaestro2\\Desktop\\w17Changes\\MaestroRun\\maestro_run\\BbClar.ff.C4B4-10-440.msm");
		engine.soundStart(index, initialGain);
		//}
/*
		catch(std::exception &e){
			std::cout << e.what() << std::endl;
		}
		*/
	}

	void updateGain_alias(double targetGain) {
		engine.updateGain(targetGain);
	}

	//call this to end a sound
	void soundRelease_alias() {
		engine.soundRelease();
	}

	void soundStop_alias() {
		engine.soundStop();
	}
