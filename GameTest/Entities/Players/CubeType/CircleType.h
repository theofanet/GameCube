//
//  CircleType.hpp
//  GameTest2
//
//  Created by Theo Fanet on 08/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef CircleType_hpp
#define CircleType_hpp


#define X_JUMP_BIG    10
#define X_JUMP_LITTLE 2
#define Y_JUMP_BIG    30
#define Y_JUMP_LITTLE 15

#define LITTLE_MAX_STEP 200.0

#define SWITCH_KEY 's'

#define C_ANGULAR_VELOCITY 2.0


#include <stdio.h>
#include "Player.h"

class CircleType : public PlayerType{
public:
    CircleType(Player *p, GLfloat radius);
    ~CircleType();
    
    b2Body* setGeometrie(b2World *world);
    
    void draw();
    void update();
    void jump();
    
    void endJump();
    
    void keyboardSpecial(int key, IoKeyStats state);
    void keyboard(unsigned char key, IoKeyStats state);
    
    void setUnactive();
    
private:
    GLfloat _radius;
    bool    _isLittle;
    int     _nbJump, _littleCount;
    
    GRender::Image *_image;
};

#endif /* CircleType_hpp */
