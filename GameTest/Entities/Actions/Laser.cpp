//
//  Laser.cpp
//  GameCube
//
//  Created by Theo Fanet on 27/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "Laser.h"


Laser::Laser(b2World *world, string name, b2Vec2 pos, GLfloat power) : Object(world, name, pos){
    _type = OBJECT_TYPE_LASER;
    
    _power = power;
    _dim   = 5 * b2Vec2(power, power);
    
    _isOn = true;
    
    _autoRotationActive = false;
    _autoStartAngle     = 0.0f;
    _autoEndAngle       = 360.0f;
    _autoStepAngle      = 0.1f;
    
    _texture[0][0] = new GRender::Image("Resources/Images/Objects/laser/laser_start_back.png");
    _texture[0][0]->setBasePosition(0, 0.01);
    _texture[0][0]->scale(0.02);
    _texture[0][0]->setBlendFunc(GL_ONE);
    
    _texture[0][1] = new GRender::Image("Resources/Images/Objects/laser/laser_mid_back.png");
    _texture[0][1]->setRepeat(1, 100);
    _texture[0][1]->scale(0.02);
    _texture[0][1]->setBlendFunc(GL_ONE);
    
    _texture[0][2] = new GRender::Image("Resources/Images/Objects/laser/laser_end_back.png");
    _texture[0][2]->scale(0.02);
    _texture[0][2]->setBlendFunc(GL_ONE);
    
    _texture[1][0] = new GRender::Image("Resources/Images/Objects/laser/laser_start_front.png");
    _texture[1][0]->setBasePosition(0, 0.01);
    _texture[1][0]->scale(0.02);
    _texture[1][0]->setBlendFunc(GL_ONE);
    
    _texture[1][1] = new GRender::Image("Resources/Images/Objects/laser/laser_mid_front.png");
    _texture[1][1]->setRepeat(1, 100);
    _texture[1][1]->scale(0.02);
    _texture[1][1]->setBlendFunc(GL_ONE);
    
    _texture[1][2] = new GRender::Image("Resources/Images/Objects/laser/laser_end_front.png");
    _texture[1][2]->scale(0.02);
    _texture[1][2]->setBlendFunc(GL_ONE);
    
    _overlay = new GRender::Image("Resources/Images/Objects/laser/laser_overlay.png");
    _overlay->setRepeat(1, 100);
    _overlay->scale(0.02);
    _overlay->setBlendFunc(GL_ONE);
    
    _baseTexture = new GRender::Image("Resources/Images/Objects/laser/laserBase.png");
    _baseTexture->scale(0.002);
    
    _color[0] = 1.0f;
    _color[1] = 0.0f;
    _color[2] = 0.0f;
    
    _backColor[0] = 0.3f;
    _backColor[1] = 0.0f;
    _backColor[2] = 0.0f;
    
    _animation = 100;
    
    _cmdList.push_back("turn_on");
    
    resetBody();
}

Laser::~Laser(){
    for(int layer = 0; layer < 2; layer++)
        for(int part = 0; part < 3; part++)
            delete_obj(_texture[layer][part]);
    delete_obj(_overlay);
    delete_obj(_baseTexture);
}

Laser* Laser::fromJsonData(b2World *w, Json::Value data, int index, string){
    string name = StringFormatter::format("laser_%i", index);
    if(data.isMember("name"))
        name = data["name"].asString();
    
    Laser *t = new Laser(w, name, b2Vec2(data["position"]["x"].asDouble(), data["position"]["y"].asDouble()), data["power"].asDouble());
    
    t->setAngle(data["angle"].asDouble());
    t->setAutoRoatation(data["autoRotation"].asBool());
    
    if(data.isMember("autoRotationMaxAngle"))
        t->setAutoRotationStartAngle(data["autoRotationMaxAngle"].asDouble());
    else if(data.isMember("autoRotationStartAngle"))
        t->setAutoRotationStartAngle(data["autoRotationStartAngle"].asDouble());
    
    if(data.isMember("autoRotationMinAngle"))
        t->setAutoRotationEndAngle(data["autoRotationMinAngle"].asDouble());
    else if(data.isMember("autoRotationEndAngle"))
        t->setAutoRotationEndAngle(data["autoRotationEndAngle"].asDouble());
    
    if(data.isMember("autoRotationSpeed"))
        t->setAutoRotationStep(data["autoRotationSpeed"].asDouble());
    else if(data.isMember("autoRotationStep"))
        t->setAutoRotationStep(data["autoRotationStep"].asDouble());
    
    t->setColor(data["color"]["r"].asDouble(), data["color"]["g"].asDouble(), data["color"]["b"].asDouble());
    
    return t;
}

void Laser::resetBody(){
    if(_body)
        _world->DestroyBody(_body);
    
    b2BodyDef bd3;
    
    bd3.type          = b2_dynamicBody;
    bd3.fixedRotation = false;
    bd3.gravityScale  = 0.0f;
    bd3.position.Set(_position.x, _position.y);
    
    _body = _world->CreateBody(&bd3);
    
    b2PolygonShape poly3;
    
    b2Vec2 points[4];
    points[0] = b2Vec2(-0.3f, 0.0f);
    points[1] = b2Vec2(-0.3f, _power*2 + (_texture[0][0]->height()/2 + _texture[0][2]->height()) * 0.02);
    points[2] = b2Vec2(0.3f, _power*2 + (_texture[0][0]->height()/2 + _texture[0][2]->height()) * 0.02);
    points[3] = b2Vec2(0.3f, 0.0f);
    
    poly3.Set(points, 4);
    _body->CreateFixture(&poly3, 1.0);
    
    for(b2Fixture *f = _body->GetFixtureList(); f != NULL; f = f->GetNext()){
        FixtureData *data = new FixtureData();
        data->laser = this;
        data->box_type = BOX_TYPE_LASER;
        f->SetUserData((void*)data);
    }
}

void Laser::draw(){
    glPushMatrix();
    
    glTranslatef(_position.x, _position.y, 0.0);
    glRotatef(_angle, 0, 0, 1);
    
    _baseTexture->show();
    
    if(_isOn){
        float r = 0.6 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(0.4)));
        
        for(int layer = 0; layer < 2; layer++){
            for(int part = 0; part < 3; part++){
                _texture[layer][part]->setColor(
                                                (layer == 0 ? _backColor[0] : _color[0]),
                                                (layer == 0 ? _backColor[1] : _color[1]),
                                                (layer == 0 ? _backColor[2] : _color[2]),
                                                (layer == 0 ? 0.8 : 1.0)*r
                                                );
                
                if(part == 1){
                    _texture[layer][part]->setBasePosition(0, _power + (_texture[layer][0]->height()*0.02)/2);
                    _texture[layer][part]->setHeight(_power, false);
                }
                else if(part == 2)
                    _texture[layer][part]->setBasePosition(0, _power*2 + (_texture[layer][0]->height()*0.02));
                
                _texture[layer][part]->show();
            }
        }
        
        _overlay->setColor(_backColor[0], _backColor[1], _backColor[2],  .7f * r);
        _overlay->setBasePosition(-0.02, (_power + (_texture[1][0]->height()*0.02)/1.6));
        _overlay->setHeight(_power + (_texture[1][0]->height()*0.014), false);
        _overlay->show(0, _animation);
        _animation -= 0.0008;
        if(_animation < 0)
            _animation = 100;
        
        if(_autoRotationActive){
            GLfloat angle = _angle + _autoStepAngle;
            if(angle > 360)
                angle = 0;
            else if(angle < 0)
                angle = 360;
            
            if(_autoStepAngle > 0){
                if(angle > _autoEndAngle){
                    if(_autoStartAngle < _autoEndAngle || angle < _autoStartAngle)
                        angle = _autoStartAngle;
                }
            }
            else if(_autoStepAngle < 0){
                if(angle < _autoEndAngle){
                    if(_autoStartAngle > _autoEndAngle || angle > _autoStartAngle)
                        angle = _autoStartAngle;
                }
            }
            
            setAngle(angle);
        }
    }
    
    glPopMatrix();
}

void Laser::handleCmd(string cmd, bool state){
    if(cmd == "turn_on")
        _isOn = state;
}