//
//  RectType.hpp
//  GameTest2
//
//  Created by Theo Fanet on 08/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef RectType_hpp
#define RectType_hpp

#include <stdio.h>
#include "Player.h"

#define R_X_JUMP 40
#define R_Y_JUMP 200

#define DIM_PLUS  0.5
#define DIM_MINUS 0.3

#define SWITCH_UP_KEY   's'
#define SWITCH_DOWN_KEY 'd'

class RectType : public PlayerType{
public:
    RectType(Player* p, b2Vec2 dim);
    ~RectType();
    
    b2Body* setGeometrie(b2World *world);
    
    void draw();
    void update();
    void jump();
    void setActive();
    
    void keyboardSpecial(int key, IoKeyStats state);
    void keyboard(unsigned char key, IoKeyStats state);
    
private:
    b2Vec2 _dim, _initDim;
};

#endif /* RectType_hpp */
