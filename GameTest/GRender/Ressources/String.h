//
//  String.h
//  AnimationTest
//
//  Created by Theo Fanet on 14/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __AnimationTest__String__
#define __AnimationTest__String__

#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

#include "defines.h"

using namespace std;

namespace GRender {
    
    class String{
    public:
        ~String();
        
        static String *getInstance();
        
        void drawString(float x, float y, string text, void **font = GLUT_BITMAP_8_BY_13);
        void setColor(GLfloat r, GLfloat v, GLfloat b);
        
    private:
        String();
        GLfloat _r, _v, _b;
        
        static String *_instance;
        
    };
    
}

#endif /* defined(__AnimationTest__String__) */
