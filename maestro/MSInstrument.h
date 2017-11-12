//
//  instrument.hpp
//  RtAudio
//
//  Created by Jungho Bang on 11/11/17.
//

#ifndef MSInstrument_h
#define MSInstrument_h

#include <stdio.h>
#include "MSModel.h"

class MSInstrument {
    MSModel *msm;
    
public:
    MSInstrument(MSModel *msm);
//    MSInstrument(std::string path);
    
    friend class MSEngine;
};

#endif
