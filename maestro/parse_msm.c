//
//  main.c
//  ReadBinary
//
//  Created by 방정호 on 3/15/17.
//  Copyright © 2017 Bangtoven. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct Model {
    double frequency;
    uint32_t partials;
    uint32_t duration;
    uint32_t loopStart; // attack - sustain
    uint32_t loopFinish; // sustain - release
    uint64_t empty[5]; // for later use...
} Model;

Model parseMSMAtPath(char* path) {
    // insert code here...
    // "samples/oboe.mf.C5B5-5-663.msm"
    FILE *file = fopen(path,"rb");
    if (!file) {
        printf("Unable to open file!");
        Model empty;
        return empty;
    }
    
    Model model;
    fread(&model, sizeof(Model), 1, file);
    printf("fundamental freq: %lf\n", model.frequency);
    printf("size: %d %d\n", model.partials, model.duration);
    
    int cells = model.partials * model.duration;
    double *mat = (double*)malloc(cells * sizeof(double));
    fread(mat, sizeof(double), cells, file);
    printf("For t=0, \n");
    double max = 0;
    for(int i=0; i<cells; i++) {
        if (mat[i] > max) max = mat[i];
        printf("%lf ", mat[i]);
        
    }
    printf("\n");
    printf("max: %lf", max);
    fclose(file);
    
    
    return model;
}
