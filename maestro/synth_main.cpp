#include "MSEngine.h"
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
    
    MSEngine s = MSEngine::sharedEngine();
    
    int i = 0;
    while (true) {
        cout << "Choose instrument to play. Enter 1 ~ 5: ";
        cin >> i;
        cin.get();
        
        if (i<1 || i>5)
            break;
        
        s.synthInit(paths[i-1]);
        
        cout << "Gain change test\n";
        s.soundStart(0.5); // initial gain = 0.5
        std::this_thread::sleep_for(std::chrono::seconds(1));
        s.updateGain(1.0);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        s.updateGain(0.1);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        s.updateGain(2.0);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        s.updateGain(0.5);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        cout << "cresendo test\n";
        for (int i=0; i<100; i++) {
            s.updateGain(0.5 + i*0.015);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        cout << "decresendo test\n";
        for (int i=0; i<100; i++) {
            s.updateGain(2.0 - i*0.015);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
//        while (true) {
//            cout << "Sustaining.... Enter new gain value (0.0 ~ 2.0): ";
//            double gain = 1.0;
//            cin >> gain;
//            cin.get();
//
//            if (gain<=0 || gain>2)
//                break;
//
//            s.updateGain(gain);
//        }
//
////        if (cin.get() == '\n')
//            cout << endl;
        
        s.soundRelease();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}

