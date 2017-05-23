//
//  GameScene.h
//  GameTest
//
//  Created by Theo Fanet on 08/03/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __GameTest__GameScene__
#define __GameTest__GameScene__

#include <stdio.h>

#include "GameSceneAbstract.h"

#include "GRender.h"
#include "DebugDraw.h"
#include "Box.h"
#include "Checkpoint.h"
#include "Listener.h"

class GameScene;

class GameSceneHandler{
public:
    virtual void goToScene(GameScene *fromScene, GameScene *toScene){};
    virtual void goNextScene(GameScene *fromScene){};
    virtual void exitScene(GameScene *fromScene){};
};

class GameScene : public GameSceneAbstract{
public:
    GameScene();
    ~GameScene();
    
    virtual void init_world(GLfloat gravity, string filename = "");
    virtual void load_world();
    
    void draw_scene();
    virtual void back_draw(){  }
    virtual void middle_draw(){  }
    virtual void front_draw(){  }
    void         draw_debug();
    virtual void after_step();
    
    GameSceneHandler *scenes_handler;
    
    void IoKeyboard(unsigned char key, IoKeyStats state);
    void IoKeyboardSpecial(int key, IoKeyStats state);
    
protected:
    DebugDraw _debug_draw;
    
    b2Vec2    _cameraPosition;
    
    bool      _draw_debug;
    bool      _center_camera;
};

#endif /* defined(__GameTest__GameScene__) */
