//
//  Box.h
//  GameTest
//
//  Created by Theo Fanet on 16/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __GameTest__Box__
#define __GameTest__Box__

#include <Box2D/Box2D.h>

#include "GRender.h"
#include "string_helper.h"

#include "defines.h"

class Box{
public:
    Box(b2World *world);
    ~Box();

    void setGeometrie(b2Vec2 pos, b2Vec2 size, bool is_static = true, bool isSphere = false);
    b2Body *body();
    
    void setImage(string filename);
    GRender::Image *image();
    
    virtual void draw();
    virtual void update();
    
    void setTexture(GLuint tex);
    
    void setBody(b2Body *body){if(_body) _world->DestroyBody(_body); _body = body;}
    b2Body* getBody(){ return _body; }
    b2World* getWorld(){ return _world; }
    b2Vec2 getPosition(){ return _body->GetPosition(); }
    void setPosition(b2Vec2 pos){ setPosition(pos.x, pos.y); }
    void setPosition(GLfloat x, GLfloat y){ _body->SetTransform(b2Vec2(x, y), _body->GetAngle()); }
    
    b2World *_world;
    b2Body  *_body;
    
protected:
    
    GRender::Image *_image;
    
    b2Vec2 _postion, _size;
    
    GLfloat _radius;
    bool _isSphere;
    
    int _box_type;
};

#endif /* defined(__GameTest__Box__) */
