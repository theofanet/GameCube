//
//  IoHandler.h
//  GameTest
//
//  Created by Theo Fanet on 02/05/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __GameTest__IoHandler__
#define __GameTest__IoHandler__

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

enum IoMouseButtons{
    IO_MOUSE_BUTTON_LEFT,
    IO_MOUSE_BUTTON_RIGHT
};

enum IoKeyStats{
    IO_KEY_UP,
    IO_KEY_DOWN
};

class IoHandlerDelegate{
public:
    virtual void IoMouseClick(b2Vec2 position, IoMouseButtons button, IoKeyStats state){};
    virtual void IoMouseMove(b2Vec2 p, bool passive){};
    virtual void IoKeyboard(unsigned char key, IoKeyStats state){};
    virtual void IoKeyboardSpecial(int key, IoKeyStats state){};
};

class IoHandler{
public:
    IoHandler();
    ~IoHandler();
    
    void setDelegate(IoHandlerDelegate *delegate);
    IoHandlerDelegate* delegate();
    
    void MouseClick(const b2Vec2& p, int button, int state);
    void MouseMove(const b2Vec2& p, bool passive = false);
    void KeyBoard(unsigned char key, int x = 0, int y = 0);
    void KeyBoardUp(unsigned char key, int x = 0, int y = 0);
    void KeyBoardSpecial(int key, int x = 0, int y = 0);
    void KeyBoardSpecialUp(int key, int x = 0, int y = 0);
    
    static IoKeyStats getKeyState(int key, bool special = false);
    static b2Vec2 getMousePosition();
    static b2Vec2 getMousePositionAtClick();
    static bool isActiveShiftModifier();
    static bool isActiveCtrlModifier();
    static bool isActiveAltModifier();
    
private:
    IoHandlerDelegate *_delegate;
    
    static IoKeyStats _key_stats[256];
    static IoKeyStats _key_special_stats[246];
    
    static b2Vec2 _mouseLastClickPosition, _mousePosition;
    
    static bool _activeShiftMod, _activeAltMod, _activeCtrlMod;
};

#endif /* defined(__GameTest__IoHandler__) */
