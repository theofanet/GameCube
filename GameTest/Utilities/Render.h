//
//  Render.h
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __GameTest__Render__
#define __GameTest__Render__


#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

#include "defines.h"

struct Vect2{
    Vect2() {x = 0; y = 0;}
    Vect2(float x, float y) : x(x), y(y) {}
    
    void SetZero() { x = 0.0f; y = 0.0f; }
    void Set(float x_, float y_) { x = x_; y = y_; }
    
    Vect2 operator -() const { Vect2 v; v.Set(-x, -y); return v; }
    
    void operator += (const Vect2& v){
        x += v.x; y += v.y;
    }
    
    void operator -= (const Vect2& v){
        x -= v.x; y -= v.y;
    }
    
    void operator *= (float a){
        x *= a; y *= a;
    }
    
    float Length() const{
        return sqrt(x * x + y * y);
    }
    
    float LengthSquared() const{
        return x * x + y * y;
    }
    
    float Normalize(){
        float length = Length();
        if(length < FLT_EPSILON)
            return 0.0f;

        float invLength = 1.0f / length;
        x *= invLength;
        y *= invLength;
        
        return length;
    }
    
    Vect2 Skew() const{
        return Vect2(-y, x);
    }
    
    float x, y;
};

inline Vect2 operator + (const Vect2& a, const Vect2& b){
    return Vect2(a.x + b.x, a.y + b.y);
}

inline Vect2 operator - (const Vect2& a, const Vect2& b){
    return Vect2(a.x - b.x, a.y - b.y);
}

inline Vect2 operator * (float s, const Vect2& a){
    return Vect2(s * a.x, s * a.y);
}

inline bool operator == (const Vect2& a, const Vect2& b){
    return a.x == b.x && a.y == b.y;
}

struct Size2{
    Size2() {w = 0; h = 0;}
    Size2(float w, float h) : w(w), h(h) {}
    
    void Set(GLfloat width, GLfloat height){
        w = width;
        h = height;
    };
    
    void operator += (const Size2& v){
        w += v.w; h += v.h;
    }
    
    void operator -= (const Size2& v){
        w -= v.w; h -= v.h;
    }
    
    void operator *= (float a){
        w *= a; h *= a;
    }
    
    GLfloat w, h;
};

struct Color4{
    Color4(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
    
    void Set(GLfloat R, GLfloat G, GLfloat B, GLfloat A = 1.0f){
        r = R;
        g = G;
        b = B;
        a = A;
    }
    
    GLfloat r, g, b, a;
};

using namespace std;

class Render{
public:
    Render();
    ~Render();
    
    void DrawString(float x, float y, string text, void **font = GLUT_BITMAP_8_BY_13);
    void DrawString(const Vect2& p, string text, void **font = GLUT_BITMAP_8_BY_13);
    
    void DrawBox(const Vect2 &p, const Size2& s, const Color4& c);
	void DrawTexturedBox(const Vect2 &p, const Size2& s, GLuint texture);
    void DrawBackground(const Vect2 &p, const Size2& s, GLuint texture);
    
    void SetTextColor(const Color4 color){
        _text_color = color;
    };


protected:
    Color4 _text_color;
};


#endif /* defined(__GameTest__Render__) */
