//
//  main.c
//  ReadBinary
//
//  Created by Jungho Bang on 3/15/17.
//  Copyright © 2017 Bangtoven. All rights reserved.
//

#include "MSModel.h"

typedef struct {
    double frequency;
    uint32_t partials;
    uint32_t duration;
    uint32_t start; // attack - sustain
    uint32_t finish; // sustain - release
    uint64_t empty[5]; // for later use...
} Metadata;

MSModel::MSModel(std::string path) {
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
    this->sustainStart = meta.start;
    this->sustainFinish = meta.finish;
//    printf("fundamental freq: %lf\n", model.frequency);
//    printf("size: %d %d\n", model.partials, model.duration);
    
    int cells = meta.partials * meta.duration;
    amplitudes = (double*)malloc(cells * sizeof(double));
    fread(amplitudes, sizeof(double), cells, file);
    
    fclose(file);
}

double* MSModel::amplitudeForTime(int t) {
    if (t<duration)
        return &amplitudes[t*partials];
    else
        return NULL;
}
