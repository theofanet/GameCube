//
//  MenuScene.cpp
//  GameCube
//
//  Created by Theo Fanet on 14/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "MenuScene.h"


MenuScene::MenuScene() : GameScene(){
    listFolderFiles("../Resources/Maps");
    
    _menuFont = new GRender::Font("Resources/Fonts/remissis.ttf", 20);
    _menuFont->setColor(0.90234375f, 0.296875f, 0.234375f);
    
    _menuFontBlink = new GRender::Font("Resources/Fonts/remissis.ttf", 20);
    _menuFontBlink->setColor(0.90234375f, 0.296875f, 0.234375f);
    _menuFontBlink->setBlink(9);
    _menuFontBlink->setBlinkColor(0.75f, 0.22265625f, 0.16796875f);
    
    _titleFont = new GRender::Font("Resources/Fonts/remissis.ttf", 40);
    _titleFont->setColor(0.90234375f, 0.296875f, 0.234375f);    
    
    _selection = 0;
    
    _loading = false;
    
    _emitter = new ParticleEmitter(m_world, "test", b2Vec2(0, 0), -45);
    _emitter->setAndleDist(0);
    _emitter->setSpeed(10.0);
    _emitter->setSize(2.0f);
    //_emitter->setColor(0.76, 0.25, 0.12);
    _emitter->setGravity(b2Vec2(0, -9.81));
}

MenuScene::~MenuScene(){
    delete_obj(_menuFont);
    delete_obj(_menuFontBlink);
    delete_obj(_titleFont);
    delete_obj(_emitter);
}

void MenuScene::middle_draw(){
    if(!_loading){
        _titleFont->print((m_settings->width - (4*60)) / 2, m_settings->height - 80, "MENU");
        
        _menuFont->print(10, m_settings->height - 150, "Choisir votre map :");
        
        for(size_t i = 0; i < _maps.size(); i++){
            if(_selection == i)
                _menuFontBlink->print(60, m_settings->height - 200 - i*40, _maps.at(i).c_str());
            else
                _menuFont->print(60, m_settings->height - 200 - i*40, _maps.at(i).c_str());
        }
        
        _menuFontBlink->print(20, m_settings->height - 200 - _selection*40, ">");
    }
    else
        _menuFontBlink->print(20, 20, "Chargement ...");
}

void MenuScene::listFolderFiles(string folder){
    DIR* dirp  = opendir(folder.c_str());
    
    struct dirent *dp;
    while((dp = readdir(dirp))){
        string filename = dp->d_name;
        if(filename.find(".map", (filename.length() - filename.length())) != string::npos)
            _maps.push_back(filename);
    }
    
    closedir(dirp);
}

void MenuScene::after_step(){
    if(_selection < 0)
        _selection = (int)_maps.size() - 1;
    else if(_selection > _maps.size() - 1)
        _selection = 0;
    
    _emitter->update(_elapsed_time);
}

void MenuScene::IoKeyboard(unsigned char key, IoKeyStats state){
    if(state == IO_KEY_DOWN){
        if(key == GLUT_KEY_RETURN){
            _loading = true;
            if(scenes_handler)
                scenes_handler->goNextScene(this);
        }
        else if(key == 'p'){
            _emitter->lunch(500);
        }
    }
}

void MenuScene::IoKeyboardSpecial(int key, IoKeyStats state){
    if(state == IO_KEY_DOWN){
        if(key == GLUT_KEY_UP)
            _selection--;
        else if(key == GLUT_KEY_DOWN)
            _selection++;
        else if(key == GLUT_KEY_RETURN){
            if(scenes_handler)
                scenes_handler->goNextScene(this);
        }
    }
}

string MenuScene::getSelectedMapPath(){
    if(_selection > 0 && _selection < _maps.size())
        return _maps.at(_selection);
    
    return _maps.at(0);
}