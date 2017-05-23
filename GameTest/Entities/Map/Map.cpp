//
//  Map.cpp
//  TestMap
//
//  Created by Theo Fanet on 07/05/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "Map.h"


Map::Map(string name, int w, int h, b2World *world){
    _world = world;
    _name = name;
    
    _size = b2Vec2(w, h);
    _draw_borders = false;
    
    _drawPosition = b2Vec2();
    _endPosition  = b2Vec2((w-1)*2, (h-1)*2);
    
    _endCheckpoint = new CheckPoint(world);
    _endCheckpoint->setPosition(_endPosition);
    _endCheckpoint->setImage("Resources/Images/EndItem.png");
    
    for(int i = 0; i < 100; i++)
        _nbObjectType[i] = 0;
}

Map::~Map(){
    for(int layer = 0; layer < 3; layer++){
        for(int x = 0; x < _size.x*2; x++){
            for(int y = 0; y < _size.y*2; y++){
                delete_obj(_tiles[layer][make_pair(x, y)]);
            }
        }
    }
    
    for(int x = 0; x < _size.x*2; x++){
        for(int y = 0; y < _size.y*2; y++){
            delete_obj(_checkpoints[make_pair(x, y)]);
        }
    }
    
    for(size_t i =0; i < _tileSets.size(); i++)
        delete_obj(_tileSets.at(i));
    
    for(map<string, Object*>::iterator it = _objects.begin(); it != _objects.end(); ++it)
        delete_obj(it->second);
}

void Map::draw(){
    int width  = floor(_viewSize.x) + 2;
    int height = floor(_viewSize.y) + 2;
    
    for(int x = _drawPosition.x - width; x < _drawPosition.x + width; x++){
        for(int y = _drawPosition.y - height; y < _drawPosition.y + height; y++){
            if(_tiles[1][make_pair(x, y)] && !_tiles[1][make_pair(x, y)]->isDestroyed())
                _tiles[1][make_pair(x, y)]->show();
        }
    }
    
    for(int x = _drawPosition.x - width; x < _drawPosition.x + width; x++){
        for(int y = _drawPosition.y - height; y < _drawPosition.y + height; y++){
            if(_checkpoints[make_pair(x, y)])
                _checkpoints[make_pair(x, y)]->draw();
        }
    }
    
    Rect viewRect;
    viewRect.min = b2Vec2(-width, -height) + _drawPosition;
    viewRect.max = b2Vec2(width, height) + _drawPosition;
    BBox viewBox(viewRect);

    for(map<string, Object*>::iterator it = _objects.begin(); it != _objects.end(); ++it)
        if(it->second->isInView(viewBox))
            it->second->draw();
    
    _endCheckpoint->draw();
}

void Map::drawFrontLayer(){
    int width  = floor(_viewSize.x) + 2;
    int height = floor(_viewSize.y) + 2;
    
    for(int x = _drawPosition.x - width; x < _drawPosition.x + width; x++){
        for(int y = _drawPosition.y - height; y < _drawPosition.y + height; y++){
            if(_tiles[2][make_pair(x, y)] && !_tiles[2][make_pair(x, y)]->isDestroyed())
                _tiles[2][make_pair(x, y)]->show();
        }
    }
}

void Map::drawBackLayer(){
    int width  = floor(_viewSize.x) + 2;
    int height = floor(_viewSize.y) + 2;
    
    for(int x = _drawPosition.x - width; x < _drawPosition.x + width; x++){
        for(int y = _drawPosition.y - height; y < _drawPosition.y + height; y++){
            if(_tiles[0][make_pair(x, y)] && !_tiles[0][make_pair(x, y)]->isDestroyed())
                _tiles[0][make_pair(x, y)]->show();
        }
    }
}

void Map::update(GLfloat dt){
    int width  = floor(_viewSize.x) + 2;
    int height = floor(_viewSize.y) + 2;
    
    Rect viewRect;
    viewRect.min = b2Vec2(-width, -height) + _drawPosition;
    viewRect.max = b2Vec2(width, height) + _drawPosition;
    BBox viewBox(viewRect);
    
    if(_toDestory.size()){
        for(vector<string>::iterator it = _toDestory.begin(); it != _toDestory.end(); ++it)
            removeObject(*it);
        _toDestory.clear();
    }
    
    for(map<string, Object*>::iterator it = _objects.begin(); it != _objects.end(); ++it)
        if(it->second->isInView(viewBox))
            it->second->update(dt);
}

void Map::addTile(MapTile *t, int layer){
    _tiles[layer][make_pair(t->getPosition().x, t->getPosition().y)] = t;
}

MapTile* Map::getTile(int x, int y, int layer){
    return _tiles[layer][make_pair(x, y)];
}

void Map::addCheckpoint(CheckPoint *c){
    _checkpoints[make_pair(c->getPosition().x, c->getPosition().y)] = c;
}

void Map::removeObject(string name){
    map<string, Object*>::iterator it = _objects.find(name);
    if(it != _objects.end()){
        delete_obj(it->second);
        _objects.erase(it);
    }
}

Object* Map::getObject(string name){
    map<string, Object*>::iterator it = _objects.find(name);
    if(it != _objects.end())
        return it->second;
    
    return NULL;
}

void Map::destroyObjectSlot(Object *obj){
    _toDestory.push_back(obj->getName());
}