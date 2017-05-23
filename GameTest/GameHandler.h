//
//  GameHandler.h
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __GameTest__GameHandler__
#define __GameTest__GameHandler__

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <iostream>
#include <stdarg.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <vector>

#include "defines.h"

#include "IoHandler.h"

#include "MenuScene.h"
#include "CubeWord.h"

using namespace std;

class GameHandler : public GameSceneHandler{
public:
    GameHandler();
    ~GameHandler();
    
    void run();
    
    b2Vec2 ConvertScreenToWorld(int x, int y);
    
    void reshape(int w, int h);
    void display(void);
    void idle(void);
    void keyboard(unsigned char key, int x = 0, int y = 0);
    void keyboard_up(unsigned char key, int x = 0, int y = 0);
    void specialKeyboard(int key, int x = 0, int y = 0);
    void specialKeyboard_up(int key, int x = 0, int y = 0);
    void mouse_click(int button, int state, int x = 0, int y = 0);
    void mouse_motion(int x, int y);
    void mouse_passive_motion(int x, int y);
    
    void setCurrentScene(GameScene *s);
    void goToScene(GameScene *fromScene, GameScene *toScene);
    void goNextScene(GameScene *fromScene);
    void exitScene(GameScene *fromScene);
    
    int  handleFPS();
    void quit_action(int code = 0);
    
private:
    int _frameCount, _currentTime, _previousTime;
    float _fps;
    
    float _width, _height;
    
    int                  _current_scene_index;
    GameScene           *_current_scene, *_to_remove;
    vector<GameScene *>  _scenes;
    
    MenuScene *_menuScene;
    
    IoHandler *_Io;
};

#endif /* defined(__GameTest__GameHandler__) */
