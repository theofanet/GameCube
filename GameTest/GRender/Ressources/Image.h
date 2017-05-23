//
//  Image.h
//  AnimationTest
//
//  Created by Theo Fanet on 14/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __AnimationTest__Image__
#define __AnimationTest__Image__

#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#include "defines.h"
#include "SOIL2.h"

using namespace std;

namespace GRender {
    class Render;
    
    class Image{
    public:
        Image();
        Image(string filename);
        ~Image();
        
        void load(string filename);
        void show(GLfloat x = 0.0, GLfloat y = 0.0);
        void scale(GLfloat s);
        void rotate(GLfloat r);
        void setTexture(GLuint tex);
        
        GLuint texture();
        GLfloat width();
        GLfloat height();
        GLfloat getScale(){ return _scale; }
        
        void setHeight(GLfloat h, bool doRatio = true);
        void setWidth(GLfloat w, bool doRatio = true);
        
        void setRepeat(int x, int y);
        
        void setBasePosition(b2Vec2 p){ _basePosition = p; };
        void setBasePosition(GLfloat x, GLfloat y){ setBasePosition(b2Vec2(x, y)); };
        
        static void staticDraw(GLuint texture, GLfloat x, GLfloat y, b2Vec2* vertices, int32 vertexCount);
        
        void continueDownWithColor(b2Color color){ _continueDown = true; _continueDownColor = color; }
        void continueDownWithColor(GLfloat r, GLfloat g, GLfloat b){ continueDownWithColor(b2Color(r, g, b)); }
        
        void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f){
            _color[0] = r;
            _color[1] = g;
            _color[2] = b;
            _color[3] = a;
        }
        
        void setBlendFunc(GLenum func){ _blendFunc = func; }
        
        void setDrawSize(b2Vec2 s){ _drawSize = s; }
        void setDrawSize(GLfloat x, GLfloat y){ setDrawSize(b2Vec2(x, y)); }
        
    private:
        GLuint _texture;
        
        GLfloat _scale;
        GLfloat _rotate;
        
        b2Vec2 _basePosition;
        
        GLfloat _width, _height;
        int _repeat_x, _repeat_y;
        
        GLfloat _color[4];
        
        bool _continueDown;
        b2Color _continueDownColor;
        
        GLenum _blendFunc;
        b2Vec2  _drawSize;
    };
    
}

#endif /* defined(__AnimationTest__Image__) */
