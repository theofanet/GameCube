//
//  Sprite.cpp
//  AnimationTest
//
//  Created by Theo Fanet on 14/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "Sprite.h"


namespace GRender {
    
    Sprite::Sprite(string filename, GLfloat w, GLfloat h){
        _base_image = new GRender::Image(filename);
        _animation  = NULL;
        
        _width  = w;
        _height = h;
        
        _scale  = 1;
        _rotate = 0;
        
        _row  = 3;
        _cell = 3;
        
        _drawSize = b2Vec2(0, 0);
    }
    
    Sprite::~Sprite(){
        map<std::string, GRender::AnimationAbstract*>::iterator it;
        for(it = _animations.begin(); it != _animations.end(); ++it)
            delete_obj(it->second);
        delete_obj(_base_image);
    }
    
    void Sprite::scale(GLfloat s){
        _scale = s;
    }
    
    void Sprite::setCell(int cell){
        if(cell > _base_image->width() / _width)
            cell = _base_image->width() / _width;
        
        _cell = cell;
    }
    
    void Sprite::setRow(int row){
        if(row > _base_image->height() / _height)
            row = _base_image->height() / _height;
        
        _row = row;
    }
    
    bool Sprite::hasAnimation(string key){
        return (_animations.find(key) != _animations.end());
    }
    
    void Sprite::addAnimation(string key, GRender::AnimationAbstract *anim){
        if(!hasAnimation(key)){
            anim->setIdentifier(key);
            _animations[key] = anim;
        }
    }
    
    void Sprite::removeAnimation(string key){
        if(hasAnimation(key))
            _animations.erase(key);
    }
    
    void Sprite::activateAnimation(string key, bool reset){
        if(hasAnimation(key)){
            setAnimation(_animations[key]);
            if(reset)
                _animation->reset();
        }
    }
    
    void Sprite::setAnimation(GRender::AnimationAbstract *anim){
        _animation = anim;
        _row       = anim->getRow();
    }
    
    AnimationAbstract *Sprite::getAnimation(string key){
        if(hasAnimation(key))
            return _animations[key];
        
        return NULL;
    }
    
    AnimationAbstract *Sprite::animation(){
        return _animation;
    }
    
    void Sprite::update(){
        if(_animation){
            _animation->update();
            setCell(_animation->getIndex());
        }
    }
    
    void Sprite::show(GLfloat x, GLfloat y, bool flip){
        glPushMatrix();
        glTranslatef(x, y, 0);
        glScaled(_scale, _scale, _scale);
        
        if(flip)
            glRotated(180, 0, 1, 0);
        
        glColor3f(1, 1, 1);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glBindTexture(GL_TEXTURE_2D, _base_image->texture());
        
        GLfloat bind_x_min = (_width*_cell) / _base_image->width();
        GLfloat bind_x_max = (_width*_cell + _width) / _base_image->width();
        GLfloat bind_y_min = (_height*_row) / _base_image->height();
        GLfloat bind_y_max = (_height*_row + _height) / _base_image->height();
        
        // Draw a textured quad
        GLfloat dw = _width;
        GLfloat dh = _height;
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
        
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        
    }

}