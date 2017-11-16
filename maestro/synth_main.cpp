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
        MSInstrument *inst = new MSInstrument(paths[i]);
        inst->msm->frequency = 200*(i+1);
        s.attachInstrument(inst);
    }
    
    for (MSInstrument *i : s.getInstruments()) {
        i->start();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    for (MSInstrument *i : s.getInstruments()) {
        i->release();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    return 0;
}

