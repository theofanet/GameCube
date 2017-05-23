//
//  RectType.cpp
//  GameTest2
//
//  Created by Theo Fanet on 08/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "RectType.h"

RectType::RectType(Player* p, b2Vec2 dim) : PlayerType(p){
    _dim = dim;
    _initDim = dim;
}

RectType::~RectType(){
    
}

b2Body* RectType::setGeometrie(b2World *world){
    b2Vec2 position = _cube->getBody() ? _cube->getBody()->GetPosition() : b2Vec2();
    
    if(_cube->getBody())
        world->DestroyBody(_cube->getBody());
    
    b2BodyDef bd3;
    
    bd3.type          = b2_dynamicBody;
    bd3.fixedRotation = false;
    bd3.position.Set(position.x, position.y);
    
    b2Body *body = world->CreateBody(&bd3);
    
    b2PolygonShape poly3;
    poly3.SetAsBox(_dim.x, _dim.y);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &poly3;
    fixtureDef.density = 0.5f;
    fixtureDef.friction = 0.9f;
    fixtureDef.restitution = 0.0f; // Make it bounce a little bit
    body->CreateFixture(&fixtureDef);
    
    b2PolygonShape footPoly;
    footPoly.SetAsBox(0.1, 0.1, b2Vec2(0, -2), 0);
    b2FixtureDef footFixtureDef;
    footFixtureDef.shape = &footPoly;
    footFixtureDef.density = 0.5f;
    footFixtureDef.friction = 0.9f;
    footFixtureDef.restitution = 0.0f; // Make it bounce a little bit
    body->CreateFixture(&footFixtureDef);
    
    for(b2Fixture *f = body->GetFixtureList(); f != NULL; f = f->GetNext()){
        FixtureData *data = new FixtureData();
        data->player = _cube;
        data->box_type    = BOX_TYPE_CUBE_RECT;
        f->SetUserData((void*)data);
    }
    
    return body;
}

void RectType::draw(){
    glPushMatrix();
    glTranslatef(_cube->getBody()->GetPosition().x, _cube->getBody()->GetPosition().y, 0);
    glRotated(_cube->getBody()->GetAngle()*RADTODEG, 0, 0, 1);
    
    glColor3f(1, 1, 1);
    
    b2PolygonShape *shape = (b2PolygonShape *)_cube->getBody()->GetFixtureList()->GetShape();
    
    // Draw a textured quad
    glBegin(GL_QUADS);
    glVertex2f(shape->GetVertex(0).x, shape->GetVertex(0).y);
    glVertex2f(shape->GetVertex(1).x, shape->GetVertex(1).y);
    glVertex2f(shape->GetVertex(2).x, shape->GetVertex(2).y);
    glVertex2f(shape->GetVertex(3).x, shape->GetVertex(3).y);
    glEnd();
    
    glPopMatrix();
}

void RectType::update(){
    if(_rotation == ROTATE_RIGHT)
        _cube->getBody()->SetAngularVelocity(-1.5);
    else if(_rotation == ROTATE_LEFT)
        _cube->getBody()->SetAngularVelocity(1.5);
    else
        _cube->getBody()->SetAngularVelocity(0);
}

void RectType::setActive(){
    _cube->getBody()->SetTransform(b2Vec2(_cube->getPosition().x, _cube->getPosition().y + _dim.y), _cube->getBody()->GetAngle());
}

void RectType::keyboardSpecial(int key, IoKeyStats state){
    if(state == IO_KEY_DOWN){
        if(key == GLUT_KEY_RIGHT)
            _rotation = ROTATE_RIGHT;
        else if(key == GLUT_KEY_LEFT)
            _rotation = ROTATE_LEFT;
        else if(key == GLUT_KEY_UP)
            jump();
    }
    else{
        if(key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT)
            _rotation = ROTATE_NONE;
    }
}

void RectType::jump(){
    _cube->getBody()->ApplyLinearImpulse(b2Vec2(_rotation*R_X_JUMP, R_Y_JUMP), _cube->getBody()->GetWorldCenter(), true);
}

void RectType::keyboard(unsigned char key, IoKeyStats state){
    if(state == IO_KEY_DOWN){
        if(key == GLUT_KEY_SPACE)
            jump();
        else if(key == SWITCH_UP_KEY){
            _dim.y += DIM_PLUS;
            _cube->_body = setGeometrie(_cube->_world);
        }
        else if(key == SWITCH_DOWN_KEY){
            _dim.y -= DIM_MINUS;
            if(_dim.y < _initDim.y)
                _dim.y = _initDim.y;
            _cube->_body = setGeometrie(_cube->_world);
        }
    }
}
