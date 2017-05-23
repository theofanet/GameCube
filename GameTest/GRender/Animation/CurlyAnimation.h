//
//  CurlyAnimation.h
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __GameTest__CurlyAnimation__
#define __GameTest__CurlyAnimation__

#include <stdio.h>
#include "Animation.h"

namespace GRender {
    
    class CurlyAnimation : public GRender::AnimationAbstract{
    public:
        CurlyAnimation(int from, int to, int delay);
        
        void update();
        
    protected:
        int _step;
    };
    
}

#endif /* defined(__GameTest__CurlyAnimation__) */
