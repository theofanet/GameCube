//
//  ParticleEmitter.cpp
//  GameCube
//
//  Created by Theo Fanet on 28/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "ParticleEmitter.h"


ParticleEmitter::ParticleEmitter(b2World *world, string name, b2Vec2 pos, GLfloat angle) : Object(world, name, pos){
    _type = OBJECT_TYPE_EMITTER;
    
    _isEmitting   = false;
    _updateRadius = true;
    _updateAlpha  = true;
    
    _angle      = angle;
    
    _open = true;
    
    _radius     = 1.0f;
    _life       = 10.0f;
    _speed      = 1.0f;
    _angleDist  = 20.0f;
    _dim        = 5*b2Vec2(_life, _life);
    
    _nbParticule = 500;
    
    _gravity = b2Vec2(0.0f, 0.0f);
    _emitSize = b2Vec2(2.0f, 0.0f);
    
    _particleTexture = new GRender::Image("Resources/Images/Objects/particleTexture.png");
    
    _graduateColor = false;
    _showTexture   = true;
    
    _color[0] = 1.0f;
    _color[1] = 0.0f;
    _color[2] = 0.0f;
    _color[3] = 1.0f;
    
    _endColor[0] = 0.0f;
    _endColor[1] = 0.0f;
    _endColor[2] = 0.0f;
    _endColor[3] = 1.0f;
    
    _cmdList.push_back("turn_on");
}

ParticleEmitter::~ParticleEmitter(){
    for(vector<Particle *>::iterator it = _particles.begin(); it != _particles.end(); ++it)
        delete_obj(*it);
    delete_obj(_particleTexture);
}

ParticleEmitter* ParticleEmitter::fromJsonData(b2World *w, Json::Value data, int index, string){
    string name = StringFormatter::format("emitter_%i", index);
    if(data.isMember("name"))
        name = data["name"].asString();
    
    ParticleEmitter *t = new ParticleEmitter(w, name, b2Vec2(data["position"]["x"].asDouble(), data["position"]["y"].asDouble()), data["angle"].asDouble());
    
    t->setLife(data["life"].asDouble());
    t->setSize(data["size"].asDouble());
    t->setAndleDist(data["angleDelta"].asDouble());
    t->setSpeed(data["speed"].asDouble());
    t->setColor(data["color"]["r"].asDouble(), data["color"]["g"].asDouble(), data["color"]["b"].asDouble());
    t->setEndColor(data["endColor"]["r"].asDouble(), data["endColor"]["g"].asDouble(), data["endColor"]["b"].asDouble());
    t->setEndColorActive(data["endColorActive"].asBool());
    t->setUpdateAlpha(data["updateAlpha"].asBool());
    t->setUpdateRadius(data["updateSize"].asBool());
    t->setEmitteSize(b2Vec2(data["emittSize"]["x"].asDouble(), data["emittSize"]["y"].asDouble()));
    t->setGravity(b2Vec2(data["gravity"]["x"].asDouble(), data["gravity"]["y"].asDouble()));
    t->setNbParticules(data["nbParticles"].asInt());
    
    return t;
}

void ParticleEmitter::particleDead(Particle *p){
    _toErease.push_back(p);
}

void ParticleEmitter::draw(){
    for(vector<Particle *>::iterator it = _particles.begin(); it != _particles.end(); ++it){
        Particle *p = *it;
        p->draw();
    }
}

void ParticleEmitter::update(GLfloat dt){
    for(vector<Particle *>::iterator it = _particles.begin(); it != _particles.end(); ++it){
        Particle *p = *it;
        p->update(dt);
    }
    
    for(size_t i = 0; i < _toErease.size(); i++){
        Particle *p = _toErease.at(i);
        std::vector<Particle *>::iterator it;
        it = std::find(_particles.begin(), _particles.end(), p);
        if(it != _particles.end()){
            _particles.erase(it);
        }
    }
    
    if(_open && _nbParticule > (int)_particles.size())
        lunch(_nbParticule);
    
    _toErease.clear();
}

void ParticleEmitter::lunch(int nb){
    if(nb > _nbParticule - (int)_particles.size())
        nb = _nbParticule - (int)_particles.size();
    
    for(int i = 0; i < nb; i++){
        Particle *p = new Particle(
                                   _position + b2Vec2(doubRand()*_emitSize.x*(doubRand() > 0.5f ? -1.0f : 1.0f), doubRand()*_emitSize.y*(doubRand() > 0.5f ? -1.0f : 1.0f)),
                                   _angle + doubRand() * _angleDist * (doubRand() > 0.5f ? -1.0f : 1.0f),
                                   _speed + doubRand(),
                                   doubRand() * _life
                                   );
        
        if(_showTexture)
            p->setTexture(_particleTexture);
        p->setColor(_color[0], _color[1], _color[2], _color[3]);
        if(_graduateColor)
            p->setEndColor(_endColor[0], _endColor[1], _endColor[2], _endColor[3]);
        p->setGravity(_gravity);
        p->setRadiusMult(_radius);
        p->setUpdateRadius(_updateRadius);
        p->setUpdateAlpha(_updateAlpha);
        p->isDead.connect(bind(&ParticleEmitter::particleDead, this, _1));
        
        _particles.push_back(p);
    }
}

void ParticleEmitter::handleCmd(string cmd, bool state){
    if(cmd == "turn_on")
        _open = state;
}