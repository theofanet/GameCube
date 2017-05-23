//
//  MapTile.cpp
//  TestMap
//
//  Created by Theo Fanet on 07/05/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "MapTile.h"


MapTile::MapTile(GRender::Sprite *sprite, b2World *world){
    _world   = world;
    
    _tileSet = sprite;
    
    _position = b2Vec2();
    _size     = b2Vec2(1, 1);
    
    _is_destructible = false;
    _is_deadly       = true;
    _is_slippery     = false;
    _is_climbable    = false;
    _destroyed       = false;
    _life            = 100;
    
    _points.push_back(b2Vec2(-1.0, -1.0));
    _points.push_back(b2Vec2(-1.0, 1.0));
    _points.push_back(b2Vec2(1.0, 1.0));
    _points.push_back(b2Vec2(1.0, -1.0));
    
    _body = NULL;
    _footFixture = NULL;
}

MapTile::~MapTile(){
    
}

void MapTile::setBody(){
    if(_body)
        _world->DestroyBody(_body);
    
    b2BodyDef bd3;
    
    bd3.type          = b2_kinematicBody;
    bd3.fixedRotation = false;
    bd3.position.Set(_position.x, _position.y);
    
    _body = _world->CreateBody(&bd3);
    b2PolygonShape poly3;
    
    b2Vec2 points[4];
    points[0] = _points.at(0);
    points[1] = _points.at(1);
    points[2] = _points.at(2);
    points[3] = _points.at(3);
    
    poly3.Set(points, 4);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &poly3;
    fixtureDef.density = 0.5f;
    fixtureDef.friction = 0.9f;
    fixtureDef.restitution = 0.0f; // Make it bounce a little bit
    _body->CreateFixture(&fixtureDef);
    
    b2PolygonShape footPoly;
    footPoly.SetAsBox(0.5, 0.5, b2Vec2(0, 1), 0);
    b2FixtureDef footFixtureDef;
    footFixtureDef.shape       = &footPoly;
    footFixtureDef.density     = 0.5f;
    footFixtureDef.friction    = 0.9f;
    footFixtureDef.restitution = 0.0f;
    footFixtureDef.isSensor    = true;
    _footFixture = _body->CreateFixture(&footFixtureDef);

    for(b2Fixture *f = _body->GetFixtureList(); f != NULL; f = f->GetNext()){
        FixtureData *data = new FixtureData();
        data->tile     = this;
        data->box_type = BOX_TYPE_MAP_TILE;
        f->SetUserData((void*)data);
    }
}

void MapTile::damage(int dmg){
    if(!_destroyed && _is_destructible){
        _life -= dmg;
        if(_life < 0)
            _life = 0;
    }
}

void MapTile::update(){
    if(_life <= 0 && !_destroyed && _is_destructible && _body){
        _world->DestroyBody(_body);
        _destroyed = true;
    }
    
    if(!_destroyed && _horizontalMove){
        if(_horizontalMoveSpeed > 0 && (_body->GetPosition().x - _position.x) > _horizontalMoveMax)
            _horizontalMoveSpeed = -_horizontalMoveSpeed;
        else if(_horizontalMoveSpeed < 0 &&  (_position.x - _body->GetPosition().x) > _horizontalMoveMax)
            _horizontalMoveSpeed = -_horizontalMoveSpeed;
        
        _body->SetLinearVelocity(b2Vec2(_horizontalMoveSpeed, 0));
    }
}

void MapTile::setHorizontalMove(bool move, GLfloat speed, GLfloat max){
    _horizontalMove = move;
    
    if(_horizontalMove){
        _horizontalMoveSpeed = speed;
        _horizontalMoveMax   = max;
    }
    else{
        _horizontalMoveSpeed = 0.0f;
        _horizontalMoveMax   = 0.0f;
        if(_body)
            _body->SetTransform(b2Vec2(), _body->GetAngle());
    }
}

void MapTile::destroy(){
    if(!_destroyed && _is_destructible && _body){
        _world->DestroyBody(_body);
        _destroyed = true;
    }
}

void MapTile::show(){
    _tileSet->setRow(_sprite_row);
    _tileSet->setCell(_sprite_cell);
    if(!_horizontalMove)
        _tileSet->show(_position.x, _position.y);
    else
        _tileSet->show(_body->GetPosition().x, _body->GetPosition().y);
}