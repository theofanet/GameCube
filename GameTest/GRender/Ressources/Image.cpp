//
//  Image.cpp
//  AnimationTest
//
//  Created by Theo Fanet on 14/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "Image.h"

namespace GRender{
    
    Image::Image(){
        _scale  = 1;
        _rotate = 0;
        
        _repeat_x = 1;
        _repeat_y = 1;
        
        _basePosition = b2Vec2(0, 0);
        
        _continueDown = false;
        
        _texture = 0;
        
        _blendFunc = GL_ONE_MINUS_SRC_ALPHA;
        
        _color[0] = 1.0f;
        _color[1] = 1.0f;
        _color[2] = 1.0f;
        _color[3] = 1.0f;
        
        
        _drawSize = b2Vec2(0, 0);
    }
    
    Image::Image(string filename) : Image(){
        load(filename);
    }
    
    void Image::load(string filename){
        string file = "../" + filename;
        GLuint tex = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
        if(!tex)
            cout << "Unable to load " << file << endl;
        
        setTexture(tex);
    }
    
    void Image::setTexture(GLuint tex){
        _texture = tex;
        
        int miplevel = 0;
        int w, h;
        
        glBindTexture(GL_TEXTURE_2D, _texture);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        _width  = (double)w;
        _height = (double)h;
    }
    
    Image::~Image(){
        //glDeleteTextures(1, &_texture);
    }
    
    void Image::scale(GLfloat s){
        _scale = s;
    }
    
    void Image::rotate(GLfloat r){
        _rotate = r;
    }
    
    GLuint Image::texture(){
        return _texture;
    }
    
    GLfloat Image::width(){
        return _width;
    }
    
    GLfloat Image::height(){
        return _height;
    }
    
    void Image::setRepeat(int x, int y){
        _repeat_x = x;
        _repeat_y = y;
    }
    
    void Image::setHeight(GLfloat h, bool doRatio){
        GLfloat ratio = h / _height;
        _height = h;
        if(doRatio)
            _width  *= ratio;
    }
    
    void Image::setWidth(GLfloat w, bool doRatio){
        GLfloat ratio = w / _width;
        _width   = w;
        if(doRatio)
            _height *= ratio;
    }
    
    void Image::show(GLfloat x, GLfloat y){
        glPushMatrix();
        glTranslatef(_basePosition.x, _basePosition.y, 0);
        glScaled(_scale, _scale, _scale);
        glRotated(_rotate, 0, 0, 1);
        
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, _blendFunc);
        
        glColor4f(_color[0], _color[1], _color[2], _color[3]);
        
        glBindTexture(GL_TEXTURE_2D, _texture);
        
        if(_repeat_x > 1 || _repeat_y > 1){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        
        GLfloat bind_x_min = x*_repeat_x;
        GLfloat bind_x_max = (1 + x)*_repeat_x;
        GLfloat bind_y_min = y*_repeat_y;
        GLfloat bind_y_max = (1 + y)*_repeat_y;
        
        // Draw a textured quad
        GLfloat dw = _width*_repeat_x;
        GLfloat dh = _height*_repeat_y;
        if(_drawSize.x && _drawSize.y){
            dw = _drawSize.x;
            dh = _drawSize.y;
        }
        
        glBegin(GL_QUADS);
        glTexCoord2f(bind_x_min, bind_y_min); glVertex2f(-dw/2, -dh/2);
        glTexCoord2f(bind_x_min, bind_y_max); glVertex2f(-dw/2, dh/2);
        glTexCoord2f(bind_x_max, bind_y_max); glVertex2f(dw/2, dh/2);
        glTexCoord2f(bind_x_max, bind_y_min); glVertex2f(dw/2, -dh/2);
        glEnd();
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        if(_continueDown){
            glColor3f(_continueDownColor.r, _continueDownColor.g, _continueDownColor.b);
            glBegin(GL_QUADS);
            glVertex2f((-_width/2)*_repeat_x, -1000);
            glVertex2f((-_width/2)*_repeat_x, 0);
            glVertex2f((_width/2)*_repeat_x, 0);
            glVertex2f((_width/2)*_repeat_x, -1000);
            glEnd();
        }
        
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    
    void Image::staticDraw(GLuint texture, GLfloat x, GLfloat y, b2Vec2* vertices, int32 vertexCount){
        
        glPushMatrix();
        glTranslatef(x, y, 0);
        
        glColor3f(1, 1, 1);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);        
        
        // Draw a textured quad
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(vertices[0].x, vertices[0].y);
        glTexCoord2f(0, 1); glVertex2f(vertices[1].x, vertices[1].y);
        glTexCoord2f(1, 1); glVertex2f(vertices[2].x, vertices[2].y);
        glTexCoord2f(1, 0); glVertex2f(vertices[3].x, vertices[3].y);
        glEnd();
        
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        
    }
    
}