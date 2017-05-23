//
//  Sprite.h
//  AnimationTest
//
//  Created by Theo Fanet on 14/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __AnimationTest__Sprite__
#define __AnimationTest__Sprite__

#include <stdio.h>
#include <map>

#include "defines.h"
#include "Image.h"
#include "Animation.h"

using namespace std;

namespace GRender {
    
    class Sprite{
    public:
        Sprite(string filename, GLfloat w, GLfloat h);
        ~Sprite();
        
        void scale(GLfloat s);
        void show(GLfloat x, GLfloat y, bool flip = false);
        void show(b2Vec2 pos, bool flip = false){ show(pos.x, pos.y, flip); }
        void update();
        
        void setRow(int row);
        void setCell(int cell);
        
        bool hasAnimation(string key);
        void removeAnimation(string key);
        void addAnimation(string key, GRender::AnimationAbstract *anim);
        void setAnimation(GRender::AnimationAbstract *anim);
        void activateAnimation(string key, bool reset = true);
        GRender::AnimationAbstract *animation();
        GRender::AnimationAbstract *getAnimation(string key);
        
        void setDrawSize(b2Vec2 s){ _drawSize = s; }
        void setDrawSize(GLfloat x, GLfloat y){ setDrawSize(b2Vec2(x, y)); }
        
        GRender::Image *getImage(){ return _base_image; }
        
    private:
        GRender::Image     *_base_image;
        
        GRender::AnimationAbstract                   *_animation;
        map<std::string, GRender::AnimationAbstract*> _animations;
        
        int _row, _cell;
        
        GLfloat _width;
        GLfloat _height;
        GLfloat _scale;
        GLfloat _rotate;
        b2Vec2  _drawSize;
    };
    
}

#endif /* defined(__AnimationTest__Sprite__) */
