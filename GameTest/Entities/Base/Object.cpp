//
//  Object.cpp
//  GameCube
//
//  Created by Theo Fanet on 31/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "Object.h"


Object::Object(b2World *world, string name, b2Vec2 pos){
    _position  = pos;
    _world = world;
    
    _sprite = NULL;
    _body   = NULL;
    
    _name = name;
    
    _mouseOver = false;
    _selected  = false;
    
    _dim       = b2Vec2(2.0f, 2.0f);
    
    _type = OBJECT_TYPE_NONE;
}

Object::~Object(){
    delete_obj(_sprite);
}

void Object::draw(){
    if(_sprite)
        _sprite->show(_position.x, _position.y);
}

BBox Object::getRectBox(){
    Rect rect;
    
    rect.min.x = -_dim.x / 2.0f;
    rect.min.y = -_dim.y / 2.0f;
    rect.max.x = _dim.x / 2.0f;
    rect.max.y = _dim.y / 2.0f;
    
    rect.min += _position;
    rect.max += _position;
    
    BBox box(rect);
    return box;
}

void Object::update(GLfloat){
    if(_sprite)
        _sprite->update();
}
