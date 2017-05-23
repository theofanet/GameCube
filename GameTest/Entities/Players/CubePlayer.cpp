//
//  CubePlayer.cpp
//  GameTest2
//
//  Created by Theo Fanet on 07/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "CubePlayer.h"

CubePlayer::CubePlayer(b2World *world) : Player(world){
    _type = NULL;
    
    _square = new SquareType(this, CUBE_DIM);
    _types[TYPE_SQUARE] = _square;
    _square->jetonUsedTimeUpdate.connect(bind(&CubePlayer::jetonUsedTimeUpdateSlot, this, _1, _2, _3));
    
    _circle = new CircleType(this, CIRCLE_DIM);
    _types[TYPE_CIRCLE] = _circle;
    _circle->jetonUsedTimeUpdate.connect(bind(&CubePlayer::jetonUsedTimeUpdateSlot, this, _1, _2, _3));
    
    _rect   = new RectType(this, RECT_DIM);
    _types[TYPE_RECT] = _rect;
    
    setType(TYPE_SQUARE);
}

CubePlayer::~CubePlayer(){
    delete_obj(_square);
    delete_obj(_circle);
    delete_obj(_rect);
}

void CubePlayer::setType(CubeType type){
    if(_types.find(type) != _types.end()){
        if(_type)
            _type->setUnactive();
        _type  = _types[type];
        __type = type;
        _type->setActive();
        _body  = _type->setGeometrie(_world);
    }
}

void CubePlayer::update(){
    _type->update();
    if(_is_dead){
        if(_lastCheckPoint)
            setPosition(_lastCheckPoint->getPosition());
        else
            setPosition(_map->getStartPosition());
        _is_dead = false;
    }
}

void CubePlayer::keyboard(unsigned char key, IoKeyStats state){
    _type->keyboard(key, state);
    
    Switch     *switch_but = NULL;
    PushButton *push_but   = NULL;
    
    for (b2ContactEdge* contactEdge = _body->GetContactList(); contactEdge; contactEdge = contactEdge->next){
        b2Contact *contact = contactEdge->contact;
        
        if(contact->IsTouching()){
            b2Fixture* fixtureA = contact->GetFixtureA();
            b2Fixture* fixtureB = contact->GetFixtureB();
            
            FixtureData* A = (FixtureData*)fixtureA->GetUserData();
            FixtureData* B = (FixtureData*)fixtureB->GetUserData();
            
            if(A->switch_but && !B->switch_but)
                switch_but = A->switch_but;
            else if(B->switch_but && !A->switch_but)
                switch_but = B->switch_but;
            
            if(A->push_but && !B->push_but)
                push_but = A->push_but;
            else if(B->push_but && !A->push_but)
                push_but = B->push_but;
        }
    }
    
    if(state && key == 'e' && switch_but)
        switch_but->toggle();
    else if(state && key == 'e' && push_but)
        push_but->push();
}

void CubePlayer::die(){
    Player::die();
}