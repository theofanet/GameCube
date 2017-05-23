//
//  ProgressBar.hpp
//  GameCube
//
//  Created by Theo Fanet on 28/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef ProgressBar_hpp
#define ProgressBar_hpp

#include <stdio.h>

#include "GRender.h"
#include "string_helper.h"
#include "defines.h"

class ProgressBar{
public:
    ProgressBar();
    ~ProgressBar();
    
    void draw();
    
    void setValue(GLfloat value){
        _value = 100 - value;
        if(_value < 0)
            _value = 100;
        else if(_value > 100)
            _value = 0;
    }
    GLfloat getValue(){ return 100 - _value; }
    
    void setPosition(b2Vec2 pos){ _position = pos; }
    
protected:
    b2Vec2  _position;
    GLfloat _value;
    
    GRender::Image *_textures[3];
};

#endif /* ProgressBar_hpp */
