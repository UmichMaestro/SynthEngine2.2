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
    
    int i = 0;
    while (true) {
        cout << "Choose instrument to play. Enter 1 ~ 5: ";
        cin >> i;
        cin.get();
        
        if (i<1 || i>5)
            break;
        
        s->synthInit(paths[i-1]);
        s->soundStart();
        
        cout << "Sustaining.... To release, press ENTER: ";
        if (cin.get() == '\n')
            cout << endl;
        
        s->soundRelease();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}

