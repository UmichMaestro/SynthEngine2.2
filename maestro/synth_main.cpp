#include "MSEngine.h"
#include <string>
#include <thread>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]) {
//    string paths[] = {
//        "Bassoon.ff.C4B4-10-444.msm",
//        "Horn.ff.C4B4-10-442.msm",
//        "oboe.ff.C4B4-10-439.msm",
//        "BbClar.ff.C4B4-10-440.msm",
//        "flute.nonvib.ff.B3B4-11-442.msm"
//    };
    
    string paths[] = {
        "flute.nonvib.mf.C5B5-3-589.msm",
        "Horn.mf.C4B4-8-394.msm",
        "BbClar.mf.C4B4-5-330.msm",
        "oboe.mf.C4B4-8-441.msm",
        "Bassoon.mf.C3B3-1-131.msm"
    };
    
    MSEngine s = MSEngine::sharedEngine();
    
    for (int i=0; i<5; i++) {
//        MSModel *msm = new MSModel(paths[i]);
        MSInstNode *inst = new MSInstNode(paths[i]);
        s.attachInstrument(inst);
    }
    
    for (MSInstNode *i : s.getInstruments()) {
        i->start(0.5);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
//    for (MSInstrument *i : s.getInstruments()) {
//        i->setGain(1.0);
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }
    
    for (MSInstNode *i : s.getInstruments()) {
        i->release();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}

