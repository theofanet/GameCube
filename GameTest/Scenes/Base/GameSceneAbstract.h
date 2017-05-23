//
//  GameSceneAbstract.h
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __GameTest__GameSceneAbstract__
#define __GameTest__GameSceneAbstract__

#include <iostream>
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#include "Box2D.h"

#include "defines.h"
#include "SOIL2.h"
#include "string_helper.h"

#include "json.h"
#include "b2dJson.h"

#include "Checkpoint.h"

#include "GRender.h"

#include "IoHandler.h"

class GameSceneAbstract : public IoHandlerDelegate {
    
public:
    GameSceneAbstract();
    virtual ~GameSceneAbstract();
    
    virtual void DrawTitle(const char *string);
    virtual void Step(int elapsed_time);
    
    virtual void center_camera(b2Vec2 pos);
    
    virtual void after_step();
    virtual void draw_scene();
    
    virtual void load_world();
    
    Settings* settings();
    void      set_settings(Settings *s);
    
    void    load_scene(string filename);
    b2Body* load_body(string filename);
    GLuint  load_texture(string filename);
    
    bool exitApp();
    
protected:
    int        m_stepCount;
    GLfloat    m_totalTime;
    Settings  *m_settings;
    bool       m_exitApp, m_nextScene;
    b2World   *m_world;
    
    GLfloat _elapsed_time;
};

#endif /* defined(__GameTest__GameSceneAbstract__) */
