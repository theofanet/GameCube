//
//  GameScene.cpp
//  GameTest
//
//  Created by Theo Fanet on 08/03/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "GameScene.h"


GameScene::GameScene() : GameSceneAbstract(){
    _draw_debug             = false;
    _center_camera          = false;
    _cameraPosition         = b2Vec2();
}

GameScene::~GameScene(){
    GameSceneAbstract::~GameSceneAbstract();
}

void GameScene::init_world(GLfloat g, string filename){
    if(!filename.empty())
        load_scene(filename);
    
    b2Vec2 gravity;
    gravity.Set(0.0f, g);
    m_world->SetGravity(gravity);
    m_world->SetDebugDraw(&_debug_draw);
}

void GameScene::load_world(){
    init_world(-9.81);
}

void GameScene::draw_scene(){
    back_draw();
    
    middle_draw();
    
    if(_draw_debug)
        draw_debug();
    
    front_draw();
}

void GameScene::draw_debug(){
    uint32 flags = 0;
    flags += 1 * b2Draw::e_shapeBit;
    flags += 0 * b2Draw::e_jointBit;
    flags += 1 * b2Draw::e_aabbBit;
    flags += 0 * b2Draw::e_centerOfMassBit;
    _debug_draw.SetFlags(flags);
    m_world->DrawDebugData();
}

void GameScene::after_step(){
    center_camera(_cameraPosition);
}

void GameScene::IoKeyboard(unsigned char key, IoKeyStats state){
}

void GameScene::IoKeyboardSpecial(int key, IoKeyStats state){
}