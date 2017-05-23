//
//  Object.hpp
//  GameCube
//
//  Created by Theo Fanet on 31/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include <iostream>
#include "sprite.h"
#include "defines.h"
#include "Listener.h"
#include "string_helper.h"
#include "Box.h"
#include "../../Utilities/json/json.h"
#include "json.h"

class Object{
public:
    enum ObjectType {
        OBJECT_TYPE_NONE = 0,
        OBJECT_TYPE_LASER,
        OBJECT_TYPE_JETON,
        OBJECT_TYPE_EMITTER,
        OBJECT_TYPE_SWITCH,
        OBJECT_TYPE_AND_DOOR,
        OBJECT_TYPE_OR_DOOR,
        OBJECT_TYPE_NO_DOOR,
        OBJECT_TYPE_PLATFORM,
        OBJECT_TYPE_PUSH_BUTTON
    };
    
    Object(b2World *world, string name, b2Vec2 pos);
    virtual ~Object();
    
    virtual void resetBody(){}
    
    b2Vec2 getPosition(){ return _position; }
    void setPosition(b2Vec2 pos){
        _position = pos;
        if(_body)
            _body->SetTransform(_position, _body->GetAngle());
    }
    
    virtual BBox getRectBox();
    
    virtual void draw();
    virtual void update(GLfloat dt = 0.0);
    
    ObjectType getType(){ return _type; }
    
    string getName(){ return _name; }
    void setName(string name){
        _name = name;
    }
    
    virtual Json::Value getJsonData(){
        Json::Value data;
        
        data["name"] = _name;
        data["type"] = _type;
        data["position"]["x"] = _position.x;
        data["position"]["y"] = _position.y;
        
        return data;
    }
    
    static Object* fromJsonData(b2World *world, Json::Value data, int index, string){
        string name = StringFormatter::format( "unknow_%i", index);
        if(data.isMember("name"))
            name = data["name"].asString();
        
        b2Vec2 pos = b2Vec2(0.0, 0.0);
        if(data.isMember("position"))
            pos = b2Vec2(data["position"]["x"].asDouble(), data["position"]["y"].asDouble());
        
        Object* o = new Object(world, name, pos);
        return o;
    }
    
    virtual bool isInView(BBox viewBox){
        return getRectBox().collide(viewBox);
    }
    
    virtual void handleCmd(string cmd, bool){}
    vector<string> getCmdList(){ return _cmdList; }
    
signals:
    Signal<void(Object*)> destroySignal;
    
protected:
    b2World *_world;
    b2Body  *_body;
    
    GRender::Sprite *_sprite;
    
    b2Vec2 _position;
    b2Vec2 _dim;
    
    ObjectType _type;
    
    vector<string> _cmdList;
    
    bool _mouseOver, _selected;
    string _name;
};

#endif /* Object_hpp */
