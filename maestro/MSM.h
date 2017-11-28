//
//  main.c
//  ReadBinary
//
//  Created by 방정호 on 3/15/17.
//  Copyright © 2017 Bangtoven. All rights reserved.
//
#ifndef MSM_H
#define MSM_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdint.h>

class MaestroSynthModel {
public:
    double frequency;
    uint32_t partials;
    uint32_t duration;
    uint32_t sustainStart;
    uint32_t sustainFinish;
    
    MaestroSynthModel(std::string path);
    double* amplitudeForTime(int t);
    
private:
    double *amplitudes;
    
};


#endif