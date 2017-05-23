//
//  Box.cpp
//  GameTest
//
//  Created by Theo Fanet on 16/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "Box.h"

Box::Box(b2World *world){
    _world    = world;
    _body     = NULL;
    _image    = NULL;
    _box_type = BOX_TYPE_NONE;
}

Box::~Box(){
    if(_image)
        delete_obj(_image);
}

void Box::setGeometrie(b2Vec2 pos, b2Vec2 size, bool is_static, bool isSphere){
    _postion = pos;
    _size    = size;
    
    _isSphere = isSphere;
    
    if(_body)
        _world->DestroyBody(_body);
    
    if(_image)
        _image->setHeight(_size.y*2);
    
    b2BodyDef bd3;
    
    bd3.type          = is_static ? b2_staticBody : b2_dynamicBody;
    bd3.fixedRotation = false;
    bd3.position.Set(pos.x, pos.y);
    
    _body = _world->CreateBody(&bd3);
    
    if(!isSphere){
        b2PolygonShape poly3;
        poly3.SetAsBox(size.x, size.y);
        _body->CreateFixture(&poly3, 1.0);
    }
    else{
        b2CircleShape sphere3;
        sphere3.m_p.Set(0, 0); //position, relative to body position
        sphere3.m_radius = size.x;
        _radius = size.x;
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &sphere3;
        fixtureDef.density = 0.5f;
        fixtureDef.friction = 0.4f;
        fixtureDef.restitution = 0.3f; // Make it bounce a little bit
        _body->CreateFixture(&fixtureDef);
    }
    
    for(b2Fixture *f = _body->GetFixtureList(); f != NULL; f = f->GetNext()){
        FixtureData *data = new FixtureData();
        data->box = this;
        data->box_type = _box_type;
        f->SetUserData((void*)data);
    }
}

void Box::draw(){
    if(_image){
        _image->setBasePosition(_postion.x, _postion.y);
        _image->show();
    }
    else{
        glPushMatrix();
        glTranslatef(_body->GetPosition().x, _body->GetPosition().y, 0);
        glRotated(_body->GetAngle()*RADTODEG, 0, 0, 1);
        
        glColor3f(1, 1, 1);
        
        if(!_isSphere){
            b2PolygonShape *shape = (b2PolygonShape *)_body->GetFixtureList()->GetShape();
            
            // Draw a textured quad
            glBegin(GL_QUADS);
            glVertex2f(shape->GetVertex(0).x, shape->GetVertex(0).y);
            glVertex2f(shape->GetVertex(1).x, shape->GetVertex(1).y);
            glVertex2f(shape->GetVertex(2).x, shape->GetVertex(2).y);
            glVertex2f(shape->GetVertex(3).x, shape->GetVertex(3).y);
            glEnd();
        }
        else{
            const float32 k_segments = 16.0f;
            const float32 k_increment = 2.0f * b2_pi / k_segments;
            float32 theta = 0.0f;
            glBegin(GL_POLYGON);
            for (int32 i = 0; i < k_segments; ++i)
            {
                b2Vec2 v = _radius * b2Vec2(cosf(theta), sinf(theta));
                glVertex2f(v.x, v.y);
                theta += k_increment;
            }
            glEnd();
        }
        
        glPopMatrix();
    }
}

void Box::setImage(string filename){
    if(_image)
        delete _image;
    
    _image = new GRender::Image(filename);
    
    if(_body){
        if(_image->height() > _size.y*2)
            _image->setHeight(_size.y*2);
        _image->setRepeat((_size.x*2)/_image->width(), (_size.y*2)/_image->height());
    }
}

void Box::setTexture(GLuint tex){
    _image->setTexture(tex);
    
    if(_body){
        if(_image->height() > _size.y*2)
            _image->setHeight(_size.y*2);
        _image->setRepeat((_size.x*2)/_image->width(), (_size.y*2)/_image->height());
    }
}

GRender::Image* Box::image(){
    return _image;
}

b2Body *Box::body(){
    return _body;
}

void Box::update(){
    
}