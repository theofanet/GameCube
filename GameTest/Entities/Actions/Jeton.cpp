//
//  Jeton.cpp
//  GameCube
//
//  Created by Theo Fanet on 27/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "Jeton.h"


Jeton::Jeton(b2World *world, string name, b2Vec2 position) : Object(world, name, position){
    _type = OBJECT_TYPE_JETON;
    
    _sprite = new GRender::Sprite("Resources/images/Objects/tileJetons.png", 85, 85);
    GRender::Animation *anim = new GRender::Animation(0, 3, 5);
    anim->setRow(0);
    anim->play();
    _sprite->setDrawSize(2, 2);
    _sprite->setAnimation(anim);
    
    resetBody();
    
    _toDestroy = false;
    _destroyed = false;
}

Jeton::~Jeton(){
    delete_obj(_sprite);
}

void Jeton::draw(){
    if(!_destroyed)
        _sprite->show(_position);
}

void Jeton::update(GLfloat dt){
    if(!_destroyed){
        _sprite->update();
        
        if(_toDestroy && _body){
            _world->DestroyBody(_body);
            _destroyed = true;
            destroySignal.emit(this);
        }
    }
}

void Jeton::resetBody(){
    if(_body)
        _world->DestroyBody(_body);
    
    b2BodyDef bd3;
    
    bd3.type          = b2_staticBody;
    bd3.fixedRotation = true;
    bd3.gravityScale  = 0.0f;
    bd3.position.Set(_position.x, _position.y);
    
    _body = _world->CreateBody(&bd3);
    
    b2PolygonShape poly3;
    poly3.SetAsBox(1.0f, 1.0f);
    _body->CreateFixture(&poly3, 1.0);
    
    for(b2Fixture *f = _body->GetFixtureList(); f != NULL; f = f->GetNext()){
        FixtureData *data = new FixtureData();
        data->jeton = this;
        data->box_type = BOX_TYPE_JETON;
        f->SetUserData((void*)data);
    }
}