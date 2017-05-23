//
//  Laser.hpp
//  GameCube
//
//  Created by Theo Fanet on 27/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef Laser_hpp
#define Laser_hpp

#include <stdio.h>
#include "defines.h"
#include "Object.h"

class Laser : public Object{
public:
    Laser(b2World *world, string name, b2Vec2 pos, GLfloat power);
    ~Laser();
    
    static Laser* fromJsonData(b2World *w, Json::Value data, int index, string);
    
    void setAngle(GLfloat a){
        _angle = a;
        if(_body)
            _body->SetTransform(_position, _angle*DEGTORAD);
    }
    
    GLfloat getAngle(){ return _angle; }
    GLfloat getPower(){ return _power; }
    
    void draw();
    
    void setColor(GLfloat r, GLfloat g, GLfloat b){
        _color[0] = r;
        _color[1] = g;
        _color[2] = b;
        _backColor[0] = (r - 0.3 < 0.0 ? 0.0 : r - 0.3);
        _backColor[1] = (g - 0.3 < 0.0 ? 0.0 : g - 0.3);
        _backColor[2] = (b - 0.3 < 0.0 ? 0.0 : b - 0.3);
    }
    
    void setColor(int index, GLfloat c){
        if(index >= 0 && index <= 2){
            _color[index]     = c;
            _backColor[index] = (c - 0.3 < 0.0 ? 0.0 : c - 0.3);
        }
    }
    
    GLfloat getColor(int index){
        if(index >= 0 && index <= 2)
            return _color[index];
        return 0.0f;
    }
    
    void setAutoRoatation(bool autoRot){ _autoRotationActive = autoRot; }
    void setAutoRotationStartAngle(GLfloat angle){ _autoStartAngle = angle; }
    void setAutoRotationEndAngle(GLfloat angle){ _autoEndAngle = angle; }
    void setAutoRotationStep(GLfloat step){ _autoStepAngle = step; }
    GLfloat getAutoRotationStep(){ return _autoStepAngle; }
    GLfloat getAutoRotationStartAngle(){ return _autoStartAngle; }
    GLfloat getAutoRotationEndAngle(){ return _autoEndAngle; }
    bool getAutoRotation(){ return _autoRotationActive; }
    
    void resetBody();
    
    void handleCmd(string cmd, bool state);
    
    bool isOn(){ return _isOn; }
    
    
protected:
    GLfloat _power;
    GLfloat _angle;
    
    GLfloat _color[3], _backColor[3];
    
    bool _autoRotationActive, _isOn;
    GLfloat _autoStartAngle, _autoEndAngle, _autoStepAngle;
    
    GLfloat         _animation, _lifeTimeOfLaser, _totalTimeOfLaser;
    GRender::Image *_texture[2][3], *_overlay, *_baseTexture;
};

#endif /* Laser_hpp */