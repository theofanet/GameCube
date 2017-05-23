//
//  Window.cpp
//  TestMap
//
//  Created by Theo Fanet on 07/05/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "Window.h"


namespace GRender { namespace UI {
    
    Window::Window() : GRender::UI::Widget(){
        _title    = "";
        _position = GRPos();
        _size     = GRSize(20, 10);
        
        _font = new GRender::Font("Resources/Fonts/Font1.ttf", 5);
    }
    
    Window::~Window(){
        delete_obj(_font);
    }
    
    void Window::draw(){
        GRender::String *str = GRender::String::getInstance();
        
        //back
        glPushMatrix();
        
        glTranslatef(_position.x, _position.y, 0);
        glColor3f(0, 1, 0);
        
        glBegin(GL_QUADS);
        glVertex2f(-_size.w/2, -_size.h/2);
        glVertex2f(-_size.w/2, _size.h/2);
        glVertex2f(_size.w/2, _size.h/2);
        glVertex2f(_size.w/2, -_size.h/2);
        glEnd();
        
        glPopMatrix();
        
        //title
        glPushMatrix();
        
        glTranslatef(_position.x + 0.1, _position.y + _size.h/2, 0);
        glColor3f(1, 1, 1);
        
        glBegin(GL_QUADS);
        glVertex2f(-_size.w/2, -2);
        glVertex2f(-_size.w/2, -0.1);
        glVertex2f(_size.w/2 - 0.2, -0.1);
        glVertex2f(_size.w/2 - 0.2, -2);
        glEnd();
        
        str->setColor(0, 0, 0);
        str->drawString(-_size.w/2 + 0.5, -1.3, _title.c_str());
        
        glPopMatrix();
        
        //content
        glPushMatrix();
        
        glTranslatef(_position.x + 0.1, _position.y + _size.h/2 - 2, 0);
        glColor3f(1, 0, 0);
        
        glBegin(GL_QUADS);
        glVertex2f(-_size.w/2, -_size.h/2 - (_size.h/2 - 2) + 0.1);
        glVertex2f(-_size.w/2, -0.1);
        glVertex2f(_size.w/2 - 0.2, -0.1);
        glVertex2f(_size.w/2 - 0.2, -_size.h/2 - (_size.h/2 - 2) + 0.1);
        glEnd();
        
        glPopMatrix();
    }
    
} }