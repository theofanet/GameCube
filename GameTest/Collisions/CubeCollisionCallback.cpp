//
//  CubeCollisionCallback.cpp
//  GameTest2
//
//  Created by Theo Fanet on 11/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "CubeCollisionCallback.h"


void CubeCollisionCallback::BeginContact(b2Contact* contact){
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    FixtureData* A = (FixtureData*)fixtureA->GetUserData();
    FixtureData* B = (FixtureData*)fixtureB->GetUserData();
    
    CubePlayer *player      = NULL;
    MapTile    *tile        = NULL;
    CheckPoint *checkpoint  = NULL;
    Laser      *laser       = NULL;
    Jeton      *jeton       = NULL;
    Platform   *platform    = NULL;
    
    if(A->player && !B->player)
        player = static_cast<CubePlayer*>(A->player);
    else if(B->player && !A->player)
        player = static_cast<CubePlayer*>(B->player);
    
    if(A->platform && !B->platform)
        platform = A->platform;
    else if(B->platform && !A->platform)
        platform = B->platform;
    
    if(A->jeton && !B->jeton)
        jeton = A->jeton;
    else if(B->jeton && !A->jeton)
        jeton = B->jeton;
    
    if(A->tile && !B->tile)
        tile = A->tile;
    else if(B->tile && !A->tile)
        tile = B->tile;
    
    if(A->laser && !B->laser)
        laser = A->laser;
    else if(B->laser && !A->laser)
        laser = B->laser;
    
    if(A->box && A->box_type == BOX_TYPE_CHECKPOINT)
        checkpoint = static_cast<CheckPoint*>(A->box);
    else if(B->box && B->box_type == BOX_TYPE_CHECKPOINT)
        checkpoint = static_cast<CheckPoint*>(B->box);
    
    if(player && checkpoint){
        player->setLastCheckPoint(checkpoint);
        player->touchedCheckpoint.emit(player, checkpoint);
        checkpoint->playerOn(player);
    }
    else if(tile && player){
        if(tile->isDeadly())
            player->die();
        if(player->getTypePlayer()->isJumping()){
            if(tile->getFootFixture() == fixtureA || tile->getFootFixture() == fixtureB)
                player->getTypePlayer()->endJump();
        }
    }
    else if(laser && player){
        if(laser->isOn())
            player->die();
    }
    else if(player && jeton && !jeton->destroyed()){
        player->addJeton();
        jeton->destroy();
    }
    else if(player && platform){
        if(player->getTypePlayer()->isJumping()){
            if(platform->getFootFixture() == fixtureA || platform->getFootFixture() == fixtureB)
                player->getTypePlayer()->endJump();
        }
    }
}

void CubeCollisionCallback::EndContact(b2Contact*){
}

void CubeCollisionCallback::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    FixtureData* A = (FixtureData*)fixtureA->GetUserData();
    FixtureData* B = (FixtureData*)fixtureB->GetUserData();
    
    CubePlayer *player     = NULL;
    CheckPoint *checkpoint = NULL;
    Laser      *laser1     = NULL;
    Laser      *laser2     = NULL;
    Jeton      *jeton      = NULL;
    
    if(A->player && !B->player)
        player = static_cast<CubePlayer*>(A->player);
    else if(B->player && !A->player)
        player = static_cast<CubePlayer*>(B->player);
    
    if(A->jeton && !B->jeton)
        jeton = A->jeton;
    else if(B->jeton && !A->jeton)
        jeton = B->jeton;
    
    if(A->box && A->box_type == BOX_TYPE_CHECKPOINT)
        checkpoint = static_cast<CheckPoint*>(A->box);
    else if(B->box && B->box_type == BOX_TYPE_CHECKPOINT)
        checkpoint = static_cast<CheckPoint*>(B->box);
    
    if(A->laser)
        laser1 = A->laser;
    else if(B->laser)
        laser1 = B->laser;
    
    if(laser1){
        if(B->laser)
            laser2 = B->laser;
    }
    
    if((player && checkpoint) || (player && laser1) || (laser1 && laser2)|| (player && jeton))
        contact->SetEnabled(false);
}