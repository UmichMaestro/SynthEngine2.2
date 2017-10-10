//
//  main.c
//  ReadBinary
//
//  Created by 방정호 on 3/15/17.
//  Copyright © 2017 Bangtoven. All rights reserved.
//

#include "MSM.h"

typedef struct {
    double frequency;
    uint32_t partials;
    uint32_t duration;
    uint32_t loopStart; // attack - sustain
    uint32_t loopFinish; // sustain - release
    uint64_t empty[5]; // for later use...
} Metadata;

MaestroSynthModel::MaestroSynthModel(std::string path) {
    FILE *file = fopen(path.c_str(),"rb");
    if (!file) {
        printf("Unable to open file!");
        return;
    }
    
    Metadata meta;
    fread(&meta, sizeof(Metadata), 1, file);
    this->frequency = meta.frequency;
    this->partials = meta.partials;
    this->duration = meta.duration;
    this->loopStart = meta.loopStart;
    this->loopFinish = meta.loopFinish;
//    printf("fundamental freq: %lf\n", model.frequency);
//    printf("size: %d %d\n", model.partials, model.duration);
    
    int cells = meta.partials * meta.duration;
    amplitudes = (double*)malloc(cells * sizeof(double));
    fread(amplitudes, sizeof(double), cells, file);
    
    fclose(file);
}

double* MaestroSynthModel::amplitudeForTime(int t) {
    if (t<duration)
        return &amplitudes[t*partials];
    else
        return NULL;
}
