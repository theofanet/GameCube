//
//  String.cpp
//  AnimationTest
//
//  Created by Theo Fanet on 14/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "String.h"

namespace GRender {
    
    String *String::_instance = new String();
    
    String::String(){
        _r = 1.0f;
        _v = 1.0f;
        _b = 1.0f;
    }
    
    String::~String(){
        
    }
    
    String* String::getInstance(){
        return _instance;
    }
    
    void String::drawString(float x, float y, string text, void **font){
        char buffer[text.length()];
        strcpy(buffer, text.c_str());
        
        glColor3f(_r, _v, _b);
        glRasterPos2f(x, y);
        
        int length = (int)strlen(buffer);
        for(int i = 0; i < length; ++i)
            glutBitmapCharacter(font, buffer[i]);
        
        glPopMatrix();
    }
    
    void String::setColor(GLfloat r, GLfloat v, GLfloat b){
        _r = r;
        _v = v;
        _b = b;
    }

}
