//
//  GameScene_first.cpp
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "GameScene_first.h"

GameScene_first::GameScene_first(){
    m_world->SetDebugDraw(&_debugDraw);
    
    b2Vec2 gravity;
    gravity.Set(0.0f, -9.81f);
    
    m_world->SetGravity(gravity);
    
    _player = new SlugPlayer(m_world);
    _player->get_body()->SetTransform(b2Vec2(-31, -12), _player->get_body()->GetAngle());
    
    _player2 = new SlugPlayer(m_world);
    _player2->get_body()->SetTransform(b2Vec2(8, -2.0), _player2->get_body()->GetAngle());
    _player2_anim = false;
    
    _player2->addToPath(PathAction::GOTO, -16, 0);
    _player2->addToPath(PathAction::GOTO, 10.10, 0);
    _player2->addToPath(PathAction::CROUCH_ON);
    _player2->addToPath(PathAction::GOTO, 27, 0);
    _player2->addToPath(PathAction::CROUCH_OFF);
    _player2->addToPath(PathAction::JUMP);
    _player2->addToPath(PathAction::GOTO, 32.26, 0);
    _player2->addToPath(PathAction::GOTO, 30, 0);
    _player2->addToPath(PathAction::JUMP);
    _player2->addToPath(PathAction::GOTO, 27, 0);
    _player2->addToPath(PathAction::JUMP);
    _player2->addToPath(PathAction::GOTO, 21, 0);
    
    _box_wall = new Box(m_world);
    _box_wall->setImage("wall.jpeg");
    _box_wall->image()->scale(0.01);
    _box_wall->image()->setRepeat(11, 1);
    _box_wall->setGeometrie(b2Vec2(.2f, -6.0f), b2Vec2(12.83f, 1.0f));
    
    _box_wall2 = new Box(m_world);
    _box_wall2->setImage("wall.jpeg");
    _box_wall2->image()->scale(0.01);
    _box_wall2->image()->setRepeat(5, 10);
    _box_wall2->setGeometrie(b2Vec2(18.0f, -2.0f), b2Vec2(5.755f, 10.95f));
    
    _box_wall3 = new Box(m_world);
    _box_wall3->setImage("wall.jpeg");
    _box_wall3->image()->scale(0.01);
    _box_wall3->image()->setRepeat(2, 1);
    _box_wall3->setGeometrie(b2Vec2(settings()->upper.x - 2.0, -9.0), b2Vec2(2.0, 1.0f));
    
    _box_wall4 = new Box(m_world);
    _box_wall4->setImage("wall.jpeg");
    _box_wall4->image()->scale(0.01);
    _box_wall4->image()->setRepeat(2, 1);
    _box_wall4->setGeometrie(b2Vec2(settings()->upper.x - 9.0, -2.0), b2Vec2(2.0, 1.0f));
    
    _box_ground = new Box(m_world);
    _box_ground->setImage("grass.jpg");
    _box_ground->image()->scale(0.01);
    _box_ground->image()->setRepeat(15, 2);
    _box_ground->setGeometrie(b2Vec2(.0f, settings()->lower.y), b2Vec2(settings()->upper.x, 9.0f));
}

GameScene_first::~GameScene_first(){
    delete _player;
    delete _player2;
    
    delete _box_ground;
    delete _box_wall;
    delete _box_wall2;
    delete _box_wall3;
    delete _box_wall4;
}

void GameScene_first::draw_scene(){
    _box_ground->draw();
    _box_wall->draw();
    _box_wall2->draw();
    _box_wall3->draw();
    _box_wall4->draw();
    
    uint32 flags = 0;
    flags += 1 * b2Draw::e_shapeBit;
    flags += 0 * b2Draw::e_jointBit;
    flags += 0 * b2Draw::e_aabbBit;
    flags += 0 * b2Draw::e_centerOfMassBit;
    _debugDraw.SetFlags(flags);
    //m_world->DrawDebugData();
    
    _player->draw();
    _player2->draw();
}

void GameScene_first::after_step(){
    _player->after_step(m_stepCount);
    
    /*if(_player->get_body()->GetPosition().x >= 309.962 && _player->get_body()->GetPosition().y <= -4){
        if(scenes_handler)
            scenes_handler->goNextScene();
    }*/
    
    _player2->after_step(m_stepCount);
    
    if(!_player2_anim && _player->get_body()->GetPosition().x >= -24.5){
        _player2_anim = true;
        _player2->LunchPath();
    }
    
    center_camera(_player->get_body()->GetPosition());
}

void GameScene_first::KeyBoard(unsigned char key, int x, int y){
    switch(key){
        case GLUT_KEY_SPACE:
            _player->jump();
            break;
        case 'c':
            _player->crouch(true);
            break;
        case 'p':
            cout << _player->get_body()->GetPosition().x << " " << _player->get_body()->GetPosition().y << endl;
            break;
        default:
            break;
    }
}

void GameScene_first::KeyBoardUp(unsigned char key, int x, int y){
    switch(key) {
        case 'c':
            _player->crouch(false);
            break;
        default:
            break;
    }
}

void GameScene_first::KeyBoardSpecial(int key, int x, int y){
    switch(key){
        case GLUT_KEY_LEFT:
            _player->set_velocity(-PLAYER_SPEED);
            break;
        case GLUT_KEY_RIGHT:
            _player->set_velocity(PLAYER_SPEED);
            break;
        default:
            break;
    }
}

void GameScene_first::KeyBoardSpecialUp(int key, int x, int y){
    
    switch(key){
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
            _player->set_velocity(0);
            break;
        default:
            break;
    }
}