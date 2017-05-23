//
//  GameScene_first.h
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __GameTest__GameScene_first__
#define __GameTest__GameScene_first__

#include <stdio.h>

#include "GRender.h"
#include "GameScene.h"
#include "SlugPlayer.h"
#include "DebugDraw.h"
#include "Box.h"


class GameScene_first : public GameScene{
public:
    GameScene_first();
    ~GameScene_first();
    
    void draw_scene();
    void after_step();
    
    void KeyBoard(unsigned char key, int x, int y);
    void KeyBoardUp(unsigned char key, int x, int y);
    void KeyBoardSpecial(int key, int x, int y);
    void KeyBoardSpecialUp(int key, int x, int y);
    
private:
    
    SlugPlayer *_player, *_player2;
    DebugDraw  _debugDraw;
    
    GRender::Image *_background_image, *_background_front_image;
    
    bool _player2_anim;
    
    Box *_box_wall, *_box_wall2, *_box_wall3, *_box_wall4, *_box_wall5, *_box_ground;
    
};

#endif /* defined(__GameTest__GameScene_first__) */
