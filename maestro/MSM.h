//
//  main.c
//  ReadBinary
//
//  Created by 방정호 on 3/15/17.
//  Copyright © 2017 Bangtoven. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string>

class MaestroSynthModel {
public:
    double frequency;
    uint32_t partials;
    uint32_t duration;
    uint32_t loopStart;
    uint32_t loopFinish;
    
    MaestroSynthModel(std::string path);
    double* amplitudeForTime(int t);
    
private:
    double *amplitudes;
    
};
