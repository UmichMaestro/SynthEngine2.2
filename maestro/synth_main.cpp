#include "synth.h"
#include <string>
#include <thread>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]) {
    string paths[] = {
        "Horn.ff.C4B4-10-442.msm",
        "oboe.ff.C4B4-10-439.msm",
        "BbClar.ff.C4B4-10-440.msm",
        "Bassoon.ff.C4B4-10-444.msm",
        "flute.nonvib.ff.B3B4-11-442.msm"
    };

    Synth *s = new Synth();
    for (string path : paths) {
        cout << path << endl;
        s->synthInit(path);
        s->soundStart();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        s->soundRelease();
    }
    
    
    
    
    return 0;
}

