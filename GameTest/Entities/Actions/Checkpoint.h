//
//  Checkpoint.h
//  GameTest
//
//  Created by Theo Fanet on 08/03/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __GameTest__Checkpoint__
#define __GameTest__Checkpoint__

#include <stdio.h>
#include <vector>

#include "Box.h"

class CheckPoint;

class CheckPointDelegate{
public:
    virtual void CheckPointSrcPlayerSet(CheckPoint *checkpoint, Player *player){};
    virtual void CheckPointDstPlayerSet(CheckPoint *checkpoint, Player *player){};
    virtual void CheckPointTriggered(CheckPoint *checkpoint){};
    virtual void CheckPointReseted(CheckPoint *checkpoint){};
    virtual void CheckPointPlayerOn(CheckPoint *checkpoint, Player *player){};
};

class CheckPoint : public Box{
public:
    CheckPoint(b2World *world);
    ~CheckPoint();
    
    void setDelegate(CheckPointDelegate *delegate){ _delegate = delegate; };
    
    void setPosition(b2Vec2 pos){setGeometrie(pos, b2Vec2(1, 1)); };
    void setGeometrie(b2Vec2 pos, b2Vec2 size, bool is_static = true);
    void setRedo(bool redo){ _redo = redo; };
    void setActive(bool active){ _active = active; };
    
    void setPlayerSrc(Player *p);
    void setPlayerDst(Player *p);
    Player *getPlayerSrc(){ return _player_source; };
    Player *getPlayerDst(){ return _player_dest; };
    
    void playerOn(Player *p);
    void performAction();
    void reset();
    
protected:
    Player *_player_dest;
    Player *_player_source;
    
    bool _triggered;
    bool _redo;
    bool _active;
    
    CheckPointDelegate *_delegate;
};

#endif /* defined(__GameTest__Checkpoint__) */
