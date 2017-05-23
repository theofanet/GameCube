//
//  Render.cpp
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "Render.h"

Render::Render() : _text_color(Color4(0.9f, 0.6f, 0.6f)){
    
}

Render::~Render(){

}

void Render::DrawString(float x, float y, string text, void **font){
    char buffer[text.length()];
    strcpy(buffer, text.c_str());
    
    glColor3f(_text_color.r, _text_color.g, _text_color.b);
    glRasterPos2f(x, y);
    
    int length = (int)strlen(buffer);
    for(int i = 0; i < length; ++i)
        glutBitmapCharacter(font, buffer[i]);
    
    glPopMatrix();
}

void Render::DrawString(const Vect2& p, string text, void **font){
    DrawString(p.x, p.y, text, font);
}

void Render::DrawBox(const Vect2 &p, const Size2& s, const Color4& c){
    glPushMatrix();
    glTranslatef(p.x, p.y, 0);
    
    glColor3f(c.r, c.g, c.b);
    
    glBegin(GL_QUADS);
    glVertex2f(-s.w, -s.h);
    glVertex2f(-s.w, s.h);
    glVertex2f(s.w, s.h);
    glVertex2f(s.w, -s.h);
    glEnd();
    
    glPopMatrix();
}

void Render::DrawTexturedBox(const Vect2 &p, const Size2& s, GLuint texture){
    glPushMatrix();
    glTranslatef(p.x, p.y, 0);
    
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Draw a textured quad
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-s.w, -s.h);
    glTexCoord2f(0, 1); glVertex2f(-s.w, s.h);
    glTexCoord2f(1, 1); glVertex2f(s.w, s.h);
    glTexCoord2f(1, 0); glVertex2f(s.w, -s.h);
    glEnd();
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Render::DrawBackground(const Vect2 &p, const Size2& s, GLuint texture){
    glPushMatrix();
    glTranslatef(p.x, p.y, 0);
    
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Draw a textured quad
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-s.w, -s.h);
    glTexCoord2f(0, 1); glVertex2f(-s.w, s.h);
    glTexCoord2f(600/8, 1); glVertex2f(s.w, s.h);
    glTexCoord2f(600/8, 0); glVertex2f(s.w, -s.h);
    glEnd();
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}