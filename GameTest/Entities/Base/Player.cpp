//
//  Player.cpp
//  GameCube
//
//  Created by Theo Fanet on 18/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "Player.h"

Player::Player(b2World* world) : Box(world){
    _lastCheckPoint = NULL;
    _is_dead        = false;
    
    _nbJetons = 1;
}

Player::~Player(){
    
}

void Player::jetonUsedTimeUpdateSlot(Player* p, int time, int time_max){
    jetonUsedTimeUpdate.emit(p, time, time_max);
}