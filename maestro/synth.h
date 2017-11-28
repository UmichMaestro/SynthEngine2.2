//
//  synth_2.1.h
//  Maestro Synth Engine
//
//  Created by Fisher Diede on 3/24/17
//  University of Michigan MDP
//
#include <string>
#include "RtAudio.h"
#include "MSM.h"




//#pragma once  



	typedef struct {
		double phase;
		int time;
		MaestroSynthModel *msm;
		bool isPlaying;

		double currentGain;
		double targetGain;
	} CallbackData;

	class Synth {
		MaestroSynthModel *msm;
		RtAudio *audio;
		RtAudio::StreamParameters *outParam;
		MaestroSynthModel *msms[5];

	public:
		CallbackData *data;

		//    Synth(std::string path);
		Synth();

		void msmInit();

		//call this function at the beginning of the program's execution to init the synth
		void synthInit();

		//call this function at the end of the program's execution to clean up the synth
		void synthDestroy();

		//call this to start a sound (call with no args for prototype)
		void soundStart(int index, double initialGain = 1.0);

		//call this to end a sound
		void soundRelease();

		void updateGain(double targetGain);

		void soundStop();

	};

	extern "C" __declspec(dllexport) void synthInit_alias();

	//call this function at the end of the program's execution to clean up the synth
	extern "C" __declspec(dllexport) void synthDestroy_alias();

	//call this to start a sound (call with no args for prototype)
	extern "C" __declspec(dllexport) void soundStart_alias(int index, double initialGain);

	extern "C" __declspec(dllexport) void updateGain_alias(double targetGain);

	//call this to end a sound
	extern "C" __declspec(dllexport) void soundRelease_alias();

	extern "C" __declspec(dllexport) void soundStop_alias();


