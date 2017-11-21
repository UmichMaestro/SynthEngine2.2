#include "MSEngine.h"
#include <string>
#include <thread>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]) {
    string paths[] = {
        "Bassoon.ff.C4B4-10-444.msm",
        "Horn.ff.C4B4-10-442.msm",
        "oboe.ff.C4B4-10-439.msm",
        "BbClar.ff.C4B4-10-440.msm",
        "flute.nonvib.ff.B3B4-11-442.msm"
    };
    
    MSEngine s = MSEngine::sharedEngine();
    
    for (int i=0; i<5; i++) {
        MSModel *msm = new MSModel(paths[i]);
        MSInstrument *inst = new MSInstrument(msm);
        s.attachInstrument(inst);
    }
    
    for (MSInstrument *i : s.getInstruments()) {
        i->start(0.5);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
//    for (MSInstrument *i : s.getInstruments()) {
//        i->setGain(1.0);
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }
    
    for (MSInstrument *i : s.getInstruments()) {
        i->release();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}

