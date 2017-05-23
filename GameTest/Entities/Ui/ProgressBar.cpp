//
//  ProgressBar.cpp
//  GameCube
//
//  Created by Theo Fanet on 28/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "ProgressBar.h"

ProgressBar::ProgressBar(){
    _textures[0] = new GRender::Image("Resources/Images/progress/progress_logo.png");
    _textures[0]->setDrawSize(b2Vec2(60.0, 60.0));
    
    _textures[1] = new GRender::Image("Resources/Images/progress/progress_back.png");
    _textures[1]->setBasePosition(b2Vec2(84.0f, -0.4f));
    _textures[1]->setDrawSize(b2Vec2(160.0f, 47.0f));

    _textures[2] = new GRender::Image("Resources/Images/progress/progress_bar.png");
    _textures[2]->setBasePosition(b2Vec2(90.0f, 0.0f));
    _textures[2]->setDrawSize(b2Vec2(125.0f, 26.0f));
    _textures[2]->setColor(0.231f, .5f, 0.7f);
    
    _position = b2Vec2(0.0f, 0.0f);
    _value    = 0.0f;
}

ProgressBar::~ProgressBar(){
    for(int part = 0; part < 3; part++)
        delete_obj(_textures[part]);
}

inline void pushScreenCoordinateMatrix() {
    glPushAttrib(GL_TRANSFORM_BIT);
    GLint	viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
    glPopAttrib();
}

inline void popScreenCoordinateMatrix() {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}


void ProgressBar::draw(){
    pushScreenCoordinateMatrix();
    
    glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glPushMatrix();
    
    glTranslated(_position.x, _position.y, 0);
    
    _textures[1]->show();
    _textures[2]->show(_value / 100.0f, 0.0f);
    _textures[0]->show();
    
    glPopMatrix();
    
    popScreenCoordinateMatrix();
}