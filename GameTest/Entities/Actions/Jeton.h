//
//  Jeton.hpp
//  GameCube
//
//  Created by Theo Fanet on 27/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef Jeton_hpp
#define Jeton_hpp

#include <stdio.h>
#include "defines.h"
#include "Object.h"

class Jeton : public Object{
public:
    Jeton(b2World *world, string name, b2Vec2 _position);
    ~Jeton();
    
    static Jeton* fromJsonData(b2World *w, Json::Value data, int index, string){
        string name = StringFormatter::format("jeton_%i", index);
        if(data.isMember("name"))
            name = data["name"].asString();
        
        b2Vec2 pos = b2Vec2(0.0, 0.0);
        if(data.isMember("position"))
            pos = b2Vec2(data["position"]["x"].asDouble(), data["position"]["y"].asDouble());
        
        Jeton* o = new Jeton(w, name, pos);
        return o;
    }
    
    virtual void handleCmd(string cmd, bool){}
    
    void draw();
    void update(GLfloat dt);
    
    void resetBody();
    
    void destroy(){ _toDestroy = true; };
    bool destroyed(){ return _toDestroy || _destroyed; }
    
protected:
    bool _toDestroy, _destroyed;
};

#endif /* Jeton_hpp */