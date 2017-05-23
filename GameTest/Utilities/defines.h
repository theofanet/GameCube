//
//  defines.h
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef GameTest_defines_h
#define GameTest_defines_h

#include <stdio.h>
#include <Box2D/Box2D.h>

#define WINDOW_TITLE "GameTest"

#define WIDTH  800
#define HEIGHT 600

#define FPS 60

#define VIEWZOOM 0.1

#define PLAYER_SPEED 4.0
#define SLUG_PLAYER_JUMP 300.0

#define GLUT_KEY_RETURN 13
#define GLUT_KEY_SPACE  ' '

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif


#define CROUCH_KEY  'c'
#define SHLASSE_KEY 'x'
#define SHOOT_KEY   's'
#define JUMP_KEY    ' '
#define LEFT_KEY    GLUT_KEY_LEFT
#define RIGHT_KEY   GLUT_KEY_RIGHT



class Player;
class Box;
class MapTile;
class CubePlayer;
class Laser;
class Jeton;
class Platform;
class Switch;
class PushButton;

struct Settings{
    Settings(){
        viewCenter.Set(0.0f, 0.0f);
        lower.Set(0, 0);
        upper.Set(0, 0);
        viewZoom = 1.0f;
        viewSize = 25.0f;
        hz       = 10.0f;
        velocityIterations = 8;
        positionIterations = 3;
        pause = 0;
        singleStep = 0;
        height = HEIGHT;
        width  = WIDTH;
        simulation_speed = 1;
        slow_down = 1;
    }
    
    b2Vec2 viewCenter;
    b2Vec2 lower;
    b2Vec2 upper;
    float viewZoom;
    float viewSize;
    float hz;
    int   velocityIterations;
    int   positionIterations;
    int   pause;
    int   singleStep;
    int   height, width;
    int   simulation_speed;
    int   slow_down;
};

#define PLAYER_TYPE_NONE  0
#define PLAYER_TYPE_SLUG  1
#define PLAYER_TYPE_CHLEU 2
#define PLAYER_TYPE_NINJA 3
#define PLAYER_TYPE_PARACHUTE 4

enum Fixture_BoxType{
    BOX_TYPE_NONE = 0,
    BOX_TYPE_GUNBALL,
    BOX_TYPE_CHECKPOINT,
    BOX_TYPE_MAP_TILE,
    BOX_TYPE_CUBE_SQUARE,
    BOX_TYPE_CUBE_RECT,
    BOX_TYPE_CUBE_CIRCLE,
    BOX_TYPE_LASER,
    BOX_TYPE_JETON,
    BOX_TYPE_PLATFORM,
    BOX_TYPE_SWITCH,
    BOX_TYPE_PUSH_BUTTON
};

#define FIXTURE_TYPE_NONE 0
#define FIXTURE_TYPE_JUMPING_OBSTACLE 2

#define FIXTURE_TYPE_PARACHUTE_OBSTACLE 3
#define FIXTURE_TYPE_PARACHUTE_GROUND   4

struct FixtureData{
    FixtureData(){
        type            = 0;
        player_type     = PLAYER_TYPE_NONE;
        player          = NULL;
        box             = NULL;
        tile            = NULL;
        laser           = NULL;
        jeton           = NULL;
        platform        = NULL;
        switch_but      = NULL;
        push_but        = NULL;
        box_type        = BOX_TYPE_NONE;
        minAngle        = -135 * DEGTORAD;
        maxAngle        = -40 * DEGTORAD;
        surfaceVelocity = PLAYER_SPEED;
    }
    
    int type;
    Player  *player;
    Box     *box;
    MapTile *tile;
    Laser   *laser;
    Jeton   *jeton;
    Platform *platform;
    Switch  *switch_but;
    PushButton *push_but;
    int player_type;
    int box_type;
    float minAngle;
    float maxAngle;
    float surfaceVelocity;
};

template<typename T>
void delete_obj(T *&ptr) {
    if(ptr)
        delete ptr;
    ptr = NULL;
}

inline float doubRand(){
    return (float)rand()/RAND_MAX;
}

struct Rect{
    b2Vec2 min, max;
};

struct BBox{
    BBox(){
        for(int i = 0; i < 4; i++){
            x[i] = 0;
            y[i] = 0;
        }
        
        rect.min = b2Vec2(0, 0);
        rect.max = b2Vec2(0, 0);
    }
    
    BBox(Rect r){
        rect = r;
        x[0] = rect.min.x;
        y[0] = rect.min.y;
        x[1] = rect.min.x;
        y[1] = rect.max.y;
        x[2] = rect.max.x;
        y[2] = rect.max.y;
        x[3] = rect.max.x;
        y[3] = rect.min.y;
    }
    
    bool collide(BBox box){
        float d1x = box.rect.min.x - rect.max.x;
        float d1y = box.rect.min.y - rect.max.y;
        float d2x = rect.min.x - box.rect.max.x;
        float d2y = rect.min.y - box.rect.max.y;
        
        if(d1x > 0.0f || d1y > 0.0f)
            return false;
        
        if(d2x > 0.0f || d2y > 0.0f)
            return false;
        
        return true;
    }
    
    float x[4];
    float y[4];
    Rect  rect;
};



#endif
