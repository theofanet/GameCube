//
//  Checkpoint.cpp
//  GameTest
//
//  Created by Theo Fanet on 08/03/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "Checkpoint.h"

CheckPoint::CheckPoint(b2World *world) : Box(world) {
    _box_type = BOX_TYPE_CHECKPOINT;
    setImage("Resources/Images/flag.png");
    
    _player_dest     = NULL;
    _player_source   = NULL;
    
    _delegate = NULL;
    
    _triggered = false;
    _redo      = false;
    _active    = true;
}

CheckPoint::~CheckPoint(){
    delete_obj(_image);
}

void CheckPoint::setGeometrie(b2Vec2 pos, b2Vec2 size, bool is_static){
    _postion = pos;
    _size    = size;
    
    if(_image)
        _image->setHeight(_size.y*2);
    
    b2BodyDef bd3;
    
    bd3.type          = is_static ? b2_staticBody : b2_dynamicBody;
    bd3.fixedRotation = false;
    bd3.position.Set(pos.x, pos.y);
    
    _body = _world->CreateBody(&bd3);
    _body->SetAwake(false);
    
    b2PolygonShape poly3;
    
    poly3.SetAsBox(size.x, size.y);
    _body->CreateFixture(&poly3, 1.0);
    
    for(b2Fixture *f = _body->GetFixtureList(); f != NULL; f = f->GetNext()){
        FixtureData *data = new FixtureData();
        data->box = this;
        data->box_type = BOX_TYPE_CHECKPOINT;
        f->SetUserData((void*)data);
    }
}

void CheckPoint::setPlayerSrc(Player *p){
    _player_source = p;
    if(_delegate)
        _delegate->CheckPointSrcPlayerSet(this, p);
}

void CheckPoint::setPlayerDst(Player *p){
    _player_dest = p;
    if(_delegate)
        _delegate->CheckPointDstPlayerSet(this, p);
}

void CheckPoint::playerOn(Player *p){
    _body->SetAwake(false);

    if(_active){
        if(_delegate)
            _delegate->CheckPointPlayerOn(this, p);
        
        if(p == _player_source)
            performAction();
    }
}

void CheckPoint::performAction(){
    if(_active) {
        if(!_triggered){
            if(_delegate)
                _delegate->CheckPointTriggered(this);
            _triggered = true;
        }
    }
}

void CheckPoint::reset(){
    if(_active){
        if(_redo && _triggered){
            _triggered = false;
            
            if(_delegate)
                _delegate->CheckPointReseted(this);
        }
    }
}