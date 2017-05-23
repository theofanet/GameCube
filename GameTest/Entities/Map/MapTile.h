//
//  MapTile.hpp
//  TestMap
//
//  Created by Theo Fanet on 07/05/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef MapTile_hpp
#define MapTile_hpp

#include "GRender.h"
#include "string_helper.h"

#include "defines.h"

class MapTile{
public:
    MapTile(GRender::Sprite *sprite, b2World *world);
    ~MapTile();
    
    void setDestructible(bool d){ _is_destructible = d; }
    void setClimbable(bool c){ _is_climbable = c; }
    void setSize(b2Vec2 s){ _size = s; }
    void setSize(GLfloat w, GLfloat h){ _size = b2Vec2(w, h); }
    void setPosition(b2Vec2 p){ _position = b2Vec2(p.x, p.y); }
    void setPosition(GLfloat x, GLfloat y){ _position = b2Vec2(x, y); }
    void destroy();
    
    bool   isDestructible(){ return _is_destructible; }
    bool   isClimbable(){ return _is_climbable; }
    bool   isDestroyed(){ return _destroyed; }
    b2Vec2 getPosition(){ return _position; }
    b2Vec2 getSize(){ return _size; }
    
    void setSlippery(bool s){ _is_slippery = s; }
    bool isSlippery(){ return _is_slippery; }
    
    void setSpritePosition(int row, int cell){ _sprite_cell = cell; _sprite_row = row; }
    int getSpriteRow(){ return _sprite_row; }
    int getSpriteCell(){ return _sprite_cell; }
    
    int getTileRow(){ return _sprite_row; }
    int getTileCell(){ return _sprite_cell; }
    
    void setBody();
    b2Body* getBody(){ return _body; }
    void damage(int dmg = 50);
    
    vector<b2Vec2> getPoints(){ return _points; }
    void setPoints(vector<b2Vec2> pts){ _points = pts; }
    void addPoint(GLfloat x, GLfloat y){ _points.push_back(b2Vec2(x, y)); }
    void clearPoints(){ _points.clear(); }
    
    void setHorizontalMove(bool move, GLfloat speed = 0.0f, GLfloat max = 0.0f);
    bool getHorizontalMove(){ return _horizontalMove; }
    GLfloat getHorizontalMoveSpeed(){ return _horizontalMoveSpeed; }
    GLfloat getHorizontalMoveMax(){ return _horizontalMoveMax; }
    
    void show();
    void update();
    
    void setDeadly(bool d){ _is_deadly = d; }
    bool isDeadly(){ return _is_deadly; }
    
    b2Fixture* getFootFixture(){ return _footFixture; }
    
protected:
    b2World *_world;
    b2Body  *_body;
    
    GRender::Sprite *_tileSet;
    
    b2Vec2 _position, _basePosition, _size;
    
    bool _is_destructible, _destroyed;
    bool _is_climbable, _is_slippery, _is_deadly;
    int  _life;
    
    int _sprite_row, _sprite_cell;
    
    bool _horizontalMove;
    GLfloat _horizontalMoveSpeed, _horizontalMoveMax;
    
    b2Fixture *_footFixture;
    
    vector<b2Vec2> _points;
};

#endif /* MapTile_hpp */
