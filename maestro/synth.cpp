//
//  Synth Wrapper for C#
//  Maestro Synth Engine
//
//  Created by Fisher Diede on 3/24/17
// 	Updated by Jungho Bang on 12/2/17.
//  University of Michigan MDP
//

#include <vector>
#include "synth.h"
#include "MSEngine.h"

void synthInit() {
	MSEngine s = MSEngine::sharedEngine();
	s.clearInstruments();
}

void synthAttachInstrument(string path) {
	MSInstNode *inst = new MSInstNode(paths);
    MSEngine s = MSEngine::sharedEngine();
	s.attachInstrument(inst);
}

void synthSoundStart(int index, double initialGain) {
	MSEngine s = MSEngine::sharedEngine();
	MSInstNode *inst = s.getInstruments()[index];
	inst->start(initialGain);
}

void synthUpdateGain(int index, double targetGain) {
	MSEngine s = MSEngine::sharedEngine();
	MSInstNode *inst = s.getInstruments()[index];
	inst->setGain(targetGain);
}

void synthSoundRelease(int index) {
	MSEngine s = MSEngine::sharedEngine();
	MSInstNode *inst = s.getInstruments()[index];
	inst->release();
}

void synthInit_prev() {
	int count = 5;
	string paths[count] = { 
		"C:\\Users\\MDPMaestro2\\Desktop\\w17Changes\\MaestroRun\\maestro_run\\instruments\\BbClar.ff.C4B4-5-330.msm" ,
		"C:\\Users\\MDPMaestro2\\Desktop\\w17Changes\\MaestroRun\\maestro_run\\instruments\\Bassoon.mf.C3B3-1-131.msm",
		"C:\\Users\\MDPMaestro2\\Desktop\\w17Changes\\MaestroRun\\maestro_run\\instruments\\flute.nonvib.ff.B3B4-11-442.msm",
		"C:\\Users\\MDPMaestro2\\Desktop\\w17Changes\\MaestroRun\\maestro_run\\instruments\\Horn.ff.C4B4-10-442.msm",
		"C:\\Users\\MDPMaestro2\\Desktop\\w17Changes\\MaestroRun\\maestro_run\\instruments\\oboe.mf.C4B4-8-441.msm"
	};

	MSEngine s = MSEngine::sharedEngine();
	s.clearInstruments(); // in case of synthInit getting called multiple times.

	for (int i=0; i<count; i++) {
        MSInstNode *inst = new MSInstNode(paths[i]);
        s.attachInstrument(inst);
    }
}
