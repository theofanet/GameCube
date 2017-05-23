//  GameSceneAbstract.cpp
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "GameSceneAbstract.h"

GameSceneAbstract::GameSceneAbstract(){
    b2Vec2 gravity;
    gravity.Set(0.0f, -9.81f);
    
    m_world    = new b2World(gravity);
    m_settings = new Settings();
    
    m_exitApp   = false;
    m_nextScene = false;
    m_stepCount = 0;
    
    float ratio = float(WIDTH) / float(HEIGHT);
    
    b2Vec2 extents(ratio * settings()->viewSize, settings()->viewSize);
    extents *= settings()->viewZoom;
    
    b2Vec2 lower = settings()->viewCenter - extents;
    b2Vec2 upper = settings()->viewCenter + extents;
    
    settings()->lower = lower;
    settings()->upper = upper;
    
    _elapsed_time = 0.0f;
    m_totalTime = 0.0f;
}

GameSceneAbstract::~GameSceneAbstract(){
    delete_obj(m_world);
    delete_obj(m_settings);
}

void GameSceneAbstract::center_camera(b2Vec2 pos){
    settings()->viewCenter = pos;
    
    glViewport(0, 0, settings()->width, settings()->height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = float(settings()->width) / float(settings()->height);
    
    b2Vec2 extents(ratio * 25.0f, 25.0f);
    extents *= settings()->viewZoom;
    
    b2Vec2 lower = settings()->viewCenter - extents;
    b2Vec2 upper = settings()->viewCenter + extents;
    
    gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
}

void GameSceneAbstract::DrawTitle(const char *string){
    GRender::String *str = GRender::String::getInstance();
    
    str->setColor(0.5f, 0.9f, 0.5f);
    str->drawString(settings()->lower.x + 2.0, settings()->upper.y - 2.0, string);
}

void GameSceneAbstract::Step(int elapsed_time){
    _elapsed_time = (double)elapsed_time / 1000.0f;
    m_totalTime += _elapsed_time;
    
    float timeStep = settings()->hz > 0.0f ? 1.0f / settings()->hz : float(0.0f);
    
    if(!m_settings->pause){
        if (timeStep > 0.0f)
            ++m_stepCount;
        
        if(m_stepCount % settings()->slow_down == 0){
            for(int i = 0; i < settings()->simulation_speed; i++){
                m_world->Step(timeStep, m_settings->velocityIterations, m_settings->positionIterations);
                after_step();
            }
        }
    }
}

void GameSceneAbstract::after_step(){
	draw_scene();
}

void GameSceneAbstract::draw_scene(){
    
}

Settings* GameSceneAbstract::settings(){
    return m_settings;
}

void GameSceneAbstract::set_settings(Settings *s){
    m_settings = s;
}

bool GameSceneAbstract::exitApp(){
    return m_exitApp;
}

GLuint GameSceneAbstract::load_texture(string filename){
    string file = "../" + filename;
    return SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
}

void GameSceneAbstract::load_world(){
    
}

void GameSceneAbstract::load_scene(string filename){
    b2dJson json;
    string errorMsg = "";
    delete m_world;
    
    string file = "../" + filename;
    m_world = json.readFromFile(file.c_str(), errorMsg);
    
    for(b2Body *b = m_world->GetBodyList(); b != NULL; b = b->GetNext()){
        for(b2Fixture *f = b->GetFixtureList(); f != NULL; f = f->GetNext()){
            FixtureData *data = new FixtureData();
            data->type = json.getCustomInt(f, "type");
            f->SetUserData((void*)data);
        }
    }
    
    if(errorMsg != "")
        cout << "error: " << errorMsg << endl;
}

b2Body* GameSceneAbstract::load_body(string filename){
    b2dJson json;
    string errorMsg = "";
    string file = "../" + filename;
    b2Body *body = json.readBodyFromFile(file.c_str(), errorMsg, m_world);
    if(errorMsg != "")
        cout << "error: " << errorMsg << endl;
    
    return body;
}