//
//  CubeWord.cpp
//  GameTest2
//
//  Created by Theo Fanet on 19/06/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "CubeWord.h"

CubeWord::CubeWord(string map_filename) : GameScene(){
    _draw_debug    = false;
    _center_camera = true;
    
    _cube   = NULL;
    _startTime = 0;
    
    m_settings->viewZoom = 0.5f;
    _wantedZoom          = 0.5f;
    
    MapJsonData data(m_world);
    data.loadFromFile("Resources/Maps/"+map_filename);
    _map = data.getMap();
    
    _menuFont = new GRender::Font("Resources/Fonts/remissis.ttf", 20);
    _menuFont->setColor(0.90234375f, 0.296875f, 0.234375f);
    
    _progress = new ProgressBar();
    _progress->setValue(100.0f);
    _progress->setPosition(b2Vec2(40.0f, 40.0f));
    
    _sceneFinish = false;
}

CubeWord::~CubeWord(){
    delete_obj(_cube);
    delete_obj(_map);
    delete_obj(m_world);
    delete_obj(_menuFont);
    delete_obj(_progress);
}

void CubeWord::load_world(){
    init_world(-9.81);
    
    _cube = new CubePlayer(m_world);
    _cube->setMap(_map);
    _cube->setGameScene(this);
    _cube->setPosition(_map->getStartPosition());
    
    _cube->emptyJeton.connect(bind(&CubeWord::NoMoreJetonSlot, this, _1));
    _cube->reloadJeton.connect(bind(&CubeWord::JetonReloadSlot, this, _1));
    _cube->jetonUsedTimeUpdate.connect(bind(&CubeWord::JetonTimeUpdateSlot, this, _1, _2, _3));
    
    _map->getEndCheckPoint()->setDelegate(this);
    _map->getEndCheckPoint()->setPlayerSrc(_cube);
    
    Box *ground = new Box(m_world);
    ground->setGeometrie(b2Vec2(0, -30), b2Vec2(1000, 20));
    
    CubeCollisionCallback *collisions_callback = new CubeCollisionCallback();
    m_world->SetContactListener(collisions_callback);
}

void CubeWord::NoMoreJetonSlot(Player*){
    _progress->setValue(0);
}

void CubeWord::JetonReloadSlot(Player*){
    _progress->setValue(100.0f);
}

void CubeWord::JetonTimeUpdateSlot(Player *, int time, int max_time){
    GLfloat value = 100.0f - ((double)time*100.0f)/(double)max_time;
    _progress->setValue(value);
}

void CubeWord::middle_draw(){
    _cube->draw();
    if(_map)
        _map->draw();
    GameScene::middle_draw();
}

void CubeWord::front_draw(){
    if(_map)
        _map->drawFrontLayer();
    GameScene::front_draw();
    
    _progress->draw();
    _menuFont->print(10, m_settings->height - 50, StringFormatter::format("Time : %f",  _finalTime).c_str());
}

void CubeWord::back_draw(){
    GameScene::back_draw();
    if(_map)
        _map->drawBackLayer();
}

void CubeWord::after_step(){
    GameScene::after_step();
    
    if(_startTime == 0)
        _startTime = m_totalTime;
    
    if(!_sceneFinish){
        _finalTime   = m_totalTime - _startTime;
        
        if(m_settings->viewZoom > _wantedZoom)
            m_settings->viewZoom -= 0.05;
        else if(m_settings->viewZoom != _wantedZoom)
            m_settings->viewZoom = _wantedZoom;
        
        _cube->update();
        _map->update(_elapsed_time);
        _map->setDrawPosition(_cube->getPosition(), m_settings->viewZoom);
        
        float ratio = float(m_settings->width) / float(m_settings->height);
        b2Vec2 extents(ratio * m_settings->viewSize, m_settings->viewSize);
        extents *= m_settings->viewZoom;
        
        _map->setViewSize(extents);
    }
    
    if(_cube && _center_camera)
        center_camera(_cube->body()->GetPosition());
}

void CubeWord::IoKeyboard(unsigned char key, IoKeyStats state){
    if(state == IO_KEY_DOWN){
        if(key == 'a')
            toggleGeo();
        else if(key == 'b')
            _map->toggleDrawBorders();
        else if(key == 27){
            if(scenes_handler)
                scenes_handler->goNextScene(this);
        }
    }
    
    if(!_sceneFinish)
        _cube->keyboard(key, state);
}

void CubeWord::IoKeyboardSpecial(int key, IoKeyStats state){
    if(!_sceneFinish)
        _cube->keyboardSpecial(key, state);
}

void CubeWord::CheckPointTriggered(CheckPoint *checkpoint){
    if(!_sceneFinish && checkpoint == _map->getEndCheckPoint()){
        _sceneFinish = true;
        m_settings->pause = 1;
    }
}

void CubeWord::toggleGeo(){
    if(_cube->getType() == TYPE_SQUARE){
        _cube->setType(TYPE_CIRCLE);
    }
    else if(_cube->getType() == TYPE_CIRCLE)
        _cube->setType(TYPE_SQUARE);
}