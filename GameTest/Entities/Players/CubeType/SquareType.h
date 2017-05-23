//
//  SquareType.hpp
//  GameTest2
//
//  Created by Theo Fanet on 07/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef SquareType_hpp
#define SquareType_hpp

#define X_JUMP 50
#define Y_JUMP 150
#define ANGULAR_VELOCITY 1.5

#define SLOW_MAX_STEP 100.0
#define SLOW_LEVEL    5

#define MAX_JUMP 3

#include <stdio.h>
#include "Player.h"

class SquareType : public PlayerType{
public:
    SquareType(Player *p, b2Vec2 dim);
    ~SquareType();
    
    b2Body* setGeometrie(b2World *world);
    
    void draw();
    void update();
    void jump();
    
    void shoot();
    
    void endJump();
    
    void keyboardSpecial(int key, IoKeyStats state);
    void keyboard(unsigned char key, IoKeyStats state);
    
    void setUnactive();
    
private:
    b2Vec2 _dim;
    bool   _showDebug, _isSlowdown;
    int    _nbJump, _slowCount;
    
    GRender::Image *_image;
};

#endif /* SquareType_hpp */
