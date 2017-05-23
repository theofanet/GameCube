//
//  Animation.h
//  AnimationTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __AnimationTest__Animation__
#define __AnimationTest__Animation__

#include <stdio.h>
#include <iostream>
#include "time.h"

#include "AnimationAbstract.h"

using namespace std;

namespace GRender {
    
    class Animation : public GRender::AnimationAbstract{
    public:
        Animation(int from, int to, int delay);
        
        void update();
    };
}

#endif /* defined(__AnimationTest__Animation__) */
