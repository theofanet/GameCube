//
//  Map.hpp
//  TestMap
//
//  Created by Theo Fanet on 07/05/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef Map_hpp
#define Map_hpp

#include <stdio.h>
#include "MapTile.h"
#include "CSVRow.h"
#include "Objects.h"

#define NB_CASE_UP    14
#define NB_CASE_RIGHT 18

using namespace std;

class Map{
public:
    Map(string name, int w, int h, b2World *world);
    ~Map();
    
    void addTileset(string filename, float w, float h){
        GRender::Sprite *tileset = new GRender::Sprite(filename, w, h);
        tileset->setDrawSize(2, 2);
        _tileSets.push_back(tileset);
    }
    
    void setSize(b2Vec2 s){ _size = s; }
    void setSize(GLfloat w, GLfloat h){ _size = b2Vec2(w, h); }
    void setDrawBorders(bool d){ _draw_borders = d; }
    void toggleDrawBorders(){ _draw_borders = !_draw_borders; }
    
    b2Vec2 getSize(){ return _size; }
    
    void addTile(MapTile *t, int layer = 1);
    MapTile* getTile(int x, int y, int layer = 1);
    void addCheckpoint(CheckPoint *c);
    
    void draw();
    void drawBackLayer();
    void drawFrontLayer();
    void update(GLfloat dt);
    
    void highlighTiles(int x, int y){_highlightTiles.push_back(make_pair(x, y));}
    
    string getName(){ return _name; }
    
    void setDrawPosition(b2Vec2 pos, GLfloat zoom = 1.0f){ _drawPosition = pos; _drawZoom = zoom; }
    void setDrawPosition(GLfloat x, GLfloat y, GLfloat zoom = 1.0f){ setDrawPosition(b2Vec2(x, y), zoom); }
    
    void setStartPosition(b2Vec2 pos){ _startPosition = pos; }
    b2Vec2 getStartPosition(){ return _startPosition; }
    
    void setEndPosition(b2Vec2 pos){ _endPosition = pos; _endCheckpoint->setPosition(pos); }
    b2Vec2 getEndPosition(){ return _endPosition; }
    
    CheckPoint *getEndCheckPoint(){ return _endCheckpoint; }
    
    vector<GRender::Sprite *> getTilesets(){ return _tileSets; }
    GRender::Sprite *getTileset(size_t i){ if(i < _tileSets.size()) return _tileSets.at(i); return NULL; }
    
    void setViewSize(b2Vec2 size){ _viewSize = size; }
    
    void removeObject(string name);
    void removeObject(Object *obj){ removeObject(obj->getName()); }
    std::map<string, Object*> getObjects(){ return _objects; }
    void addObject(Object *obj){
        _nbObjectType[obj->getType()]++;
        obj->destroySignal.connect(bind(&Map::destroyObjectSlot, this, _1));
        _objects[obj->getName()] = obj;
    }
    Object* getObject(string name);
    int getNbObjectOfType(int type){
        if(type < 100 && type >= 0)
            return _nbObjectType[type];
        return 0;
    }
    
public slots:
    void destroyObjectSlot(Object *obj);
    
protected:
    b2World *_world;
    
    string _name;
    
    b2Vec2 _size, _viewSize;
    
    b2Vec2 _drawPosition, _startPosition, _endPosition;

    float  _drawZoom;
    
    map<pair<int, int>, MapTile*> _tiles[3];
    vector<pair<int, int>> _highlightTiles;
    
    vector<GRender::Sprite *>_tileSets;
    
    map<pair<int, int>, CheckPoint*> _checkpoints;
    CheckPoint *_endCheckpoint;
    
    map<string, Object*> _objects;
    int                  _nbObjectType[100];
    
    vector<string> _toDestory;
    
    bool _draw_borders;
};

#endif /* Map_hpp */
