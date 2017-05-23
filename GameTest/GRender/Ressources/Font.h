//
//  Font.h
//  GameTest
//
//  Created by Theo Fanet on 07/05/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __GameTest__Font__
#define __GameTest__Font__

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

#include <vector>
#include <string>

#include <stdexcept>

#include "GUtils.h"
#include "defines.h"

namespace GRender {
    
    using std::vector;
    using std::string;
    
    class Font{
    public:
        Font(string filename, unsigned int h);
        ~Font();
        
        void print(float x, float y, const char *fmt, ...);
        void setColor(GLfloat r, GLfloat g, GLfloat b){ _color.set(r, g, b); };
        void setColor(GRColor color){ _color = color; };
        void setBlinkColor(GLfloat r, GLfloat g, GLfloat b){ _br = r; _bg = g; _bb = b; };
        void setBlink(int steps);
        
    private:
        float   _h;			///< Holds the height of the font.
        GLuint *_textures;	///< Holds the texture id's
        GLuint  _list_base;	///< Holds the first display list id
        
        bool    _blink;
        bool    _drawBlinked;
        int     _blinkStep;
        int     _blinkCurrentStep;
        
        GRColor _color;
        GLfloat _br, _bg, _bb;
    };
    
}


#endif /* defined(__GameTest__Font__) */
