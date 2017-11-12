//
//  main.c
//  ReadBinary
//
//  Created by Jungho Bang on 3/15/17.
//  Copyright © 2017 Bangtoven. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string>

class MSModel {
public:
    double frequency;
    uint32_t partials;
    uint32_t duration;
    uint32_t sustainStart;
    uint32_t sustainFinish;
    
    MSModel(std::string path);
    double* amplitudeForTime(int t);
    
private:
    double *amplitudes;
    
};
