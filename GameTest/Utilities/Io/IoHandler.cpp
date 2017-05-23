//
//  IoHandler.cpp
//  GameTest
//
//  Created by Theo Fanet on 02/05/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "IoHandler.h"

b2Vec2 IoHandler::_mouseLastClickPosition = b2Vec2(0, 0);
b2Vec2 IoHandler::_mousePosition          = b2Vec2(0, 0);

bool IoHandler::_activeShiftMod = false;
bool IoHandler::_activeCtrlMod  = false;
bool IoHandler::_activeAltMod   = false;

IoKeyStats IoHandler::_key_stats[256];
IoKeyStats IoHandler::_key_special_stats[246];

IoHandler::IoHandler(){
    _delegate = NULL;
    
    for(int i = 0;i < 256; i++){
        _key_stats[i] = IO_KEY_UP;
        if(i < 246)
            _key_special_stats[i] = IO_KEY_UP;
    }
    
    _mouseLastClickPosition = b2Vec2(0, 0);
    _mousePosition          = b2Vec2(0, 0);
    
    _activeShiftMod = false;
    _activeCtrlMod  = false;
    _activeAltMod   = false;
}

IoHandler::~IoHandler(){
    
}

void IoHandler::setDelegate(IoHandlerDelegate *delegate){
    _delegate = delegate;
}

IoHandlerDelegate* IoHandler::delegate(){
    return _delegate;
}

void IoHandler::MouseClick(const b2Vec2& p, int button, int state){
    _mouseLastClickPosition = p;
    _mousePosition          = p;
    
    _activeShiftMod = false;
    _activeCtrlMod  = false;
    _activeAltMod   = false;
    
    switch(glutGetModifiers()){
        case GLUT_ACTIVE_SHIFT:
            _activeShiftMod = true;
            break;
        case GLUT_ACTIVE_CTRL:
            _activeCtrlMod = true;
            break;
        case GLUT_ACTIVE_ALT:
            _activeAltMod = true;
            break;
    }
    
    if(_delegate){
        IoKeyStats     ketState    = (state == GLUT_UP ? IO_KEY_UP : IO_KEY_DOWN);
        IoMouseButtons mouseButton = (button == GLUT_RIGHT_BUTTON ? IO_MOUSE_BUTTON_RIGHT : IO_MOUSE_BUTTON_LEFT);
        
        _delegate->IoMouseClick(p, mouseButton, ketState);
    }
}

void IoHandler::MouseMove(const b2Vec2& p, bool passive){
    _mousePosition = p;
    if(_delegate)
        _delegate->IoMouseMove(p, passive);
}

void IoHandler::KeyBoard(unsigned char key, int x, int y){
    if(_delegate)
        _delegate->IoKeyboard(key, IO_KEY_DOWN);
    _key_stats[key] = IO_KEY_DOWN;
}

void IoHandler::KeyBoardUp(unsigned char key, int x, int y){
    if(_delegate)
        _delegate->IoKeyboard(key, IO_KEY_UP);
    _key_stats[key] = IO_KEY_UP;
}

void IoHandler::KeyBoardSpecial(int key, int x, int y){
    if(_delegate)
        _delegate->IoKeyboardSpecial(key, IO_KEY_DOWN);
    _key_stats[key] = IO_KEY_DOWN;
}

void IoHandler::KeyBoardSpecialUp(int key, int x, int y){
    if(_delegate)
        _delegate->IoKeyboardSpecial(key, IO_KEY_UP);
    _key_stats[key] = IO_KEY_UP;
}

IoKeyStats IoHandler::getKeyState(int key, bool special){
    if(special)
        return _key_special_stats[key];
    else
        return _key_stats[key];
}

b2Vec2 IoHandler::getMousePosition(){
    return _mousePosition;
}

b2Vec2 IoHandler::getMousePositionAtClick(){
    return _mouseLastClickPosition;
}

bool IoHandler::isActiveShiftModifier(){
    return _activeShiftMod;
}

bool IoHandler::isActiveCtrlModifier(){
    return _activeCtrlMod;
}

bool IoHandler::isActiveAltModifier(){
    return _activeAltMod;
}


