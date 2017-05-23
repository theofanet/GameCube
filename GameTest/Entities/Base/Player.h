//
//  Player.hpp
//  GameCube
//
//  Created by Theo Fanet on 18/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "GameScene.h"
#include "Box.h"
#include "Map.h"
#include "Listener.h"

enum PlayerRotate{ROTATE_NONE = 0, ROTATE_RIGHT = 1, ROTATE_LEFT = -1};

class Player;

class PlayerType{
public:
    PlayerType(Player *p){
        _cube     = p;
        _rotation = ROTATE_NONE;
        _jumping  = false;
    }
    
    ~PlayerType(){}
    
    virtual b2Body* setGeometrie(b2World *world){return NULL;}
    
    virtual void draw(){}
    virtual void update(){}
    
    virtual void jump(){ _jumping = true; }
    virtual bool isJumping(){ return _jumping; }
    virtual void endJump(){ _jumping = false; }
    
    void setCube(Player* cube){ _cube = cube; }
    
    virtual void keyboardSpecial(int key, IoKeyStats state){}
    virtual void keyboard(unsigned char key, IoKeyStats state){}
    
    void setMap(Map *map){ _map = map; }
    
    virtual void die(){}
    
    virtual void setActive(){ _rotation = ROTATE_NONE; }
    virtual void setUnactive(){}

signals:
    Signal<void(Player*, int, int)> jetonUsedTimeUpdate;
    
protected:
    Player      *_cube;
    PlayerRotate _rotation;
    
    bool _jumping;
    
    Map *_map;
};

class Player : public Box{
public:
    Player(b2World *world);
    ~Player();
    
    virtual void draw(){ }
    virtual void update(){ }
    
    virtual void keyboardSpecial(int key, IoKeyStats state){ }
    virtual void keyboard(unsigned char key, IoKeyStats state){ }
    
    void setMap(Map *map){ _map = map; };
    Map* getMap(){ return _map; }
    
    void setLastCheckPoint(CheckPoint *c){ _lastCheckPoint = c; }
    
    void setGameScene(GameScene* s){ _scene = s; }
    GameScene* getGameScene(){ return _scene; }
    
    virtual void die(){
        _is_dead = true;
        isDead.emit(this);
    }
    
    int getNbJetons(){ return _nbJetons; }
    void addJeton(){
        if(!_nbJetons)
            reloadJeton.emit(this);
        _nbJetons++;
    }
    void removeJeton(){
        if(_nbJetons){
            _nbJetons--;
        }
    }
    
signals:
    Signal<void(Player*)> isDead;
    Signal<void(Player*, CheckPoint*)> touchedCheckpoint;
    Signal<void(Player*)> emptyJeton;
    Signal<void(Player*)> reloadJeton;
    Signal<void(Player*, int, int)> jetonUsedTimeUpdate;
    
public slots:
    void jetonUsedTimeUpdateSlot(Player* p, int time, int time_max);
    
protected:
    Map        *_map;
    
    GameScene  *_scene;
    CheckPoint *_lastCheckPoint;
    
    int _nbJetons;
    
    bool _is_dead;
};

#endif /* Player_hpp */
