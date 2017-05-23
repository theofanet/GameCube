//
//  CubePlayer.hpp
//  GameTest2
//
//  Created by Theo Fanet on 07/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef CubePlayer_hpp
#define CubePlayer_hpp

#include <stdio.h>

#include "player.h"

#include "SquareType.h"
#include "CircleType.h"
#include "RectType.h"

#include "Objects.h"

#define RECT_DIM    b2Vec2(.7, 2.7)
#define CUBE_DIM    b2Vec2(1.7, 1.7)
#define CIRCLE_DIM  1.7


enum CubeType{TYPE_SQUARE, TYPE_RECT, TYPE_CIRCLE};

class CubePlayer : public Player{
public:
    CubePlayer(b2World *world);
    ~CubePlayer();
    
    void setType(CubeType type);
    CubeType getType(){ return __type; }
    PlayerType* getTypePlayer(){ return _type; }
    
    void draw(){ _type->draw(); }
    void update();
    
    void keyboardSpecial(int key, IoKeyStats state){ _type->keyboardSpecial(key, state); }
    void keyboard(unsigned char key, IoKeyStats state);
    
    void die();
    
private:
    CircleType *_circle;
    SquareType *_square;
    RectType   *_rect;
    
    PlayerType *_type;
    CubeType   __type;
    
    map<CubeType, PlayerType*> _types;
};

#endif /* CubePlayer_hpp */
