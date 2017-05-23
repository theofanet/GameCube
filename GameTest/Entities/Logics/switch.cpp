#include "switch.h"

Switch::Switch(b2World *world, string name, b2Vec2 pos) : Object(world, name, pos){
    _state = false;
    
    _type = OBJECT_TYPE_SWITCH;
    
    _img = new GRender::Image("Resources/Images/Objects/Switch.png");
    _img->setDrawSize(b2Vec2(2, 2));
    
    _cmdList.push_back("turn_on");
    
    _body = NULL;
    resetBody();
}

Switch::~Switch(){
    delete_obj(_img);
}

Json::Value Switch::getJsonData(){
    Json::Value data = Object::getJsonData();
    
    data["state"] = _state;
    int i = 0;
    for(vector<pair<Object*, string>>::iterator it = _connected.begin(); it != _connected.end(); ++it){
        data["connexion"][i]["object"] = it->first->getName();
        data["connexion"][i]["cmd"]    = it->second;
        i++;
    }
    
    return data;
}

Switch* Switch::fromJsonData(b2World *world, Json::Value data, int index, string){
    string name = StringFormatter::format("switch_%i", index);
    if(data.isMember("name"))
        name = data["name"].asString();
    
    Switch *t = new Switch(world, name, b2Vec2(data["position"]["x"].asDouble(), data["position"]["y"].asDouble()));
    
    t->setState(data["state"].asBool());
    if(data.isMember("connexion")){
        //_connexionToCreate
        int i = 0;
        Json::Value tdata = data["connexion"][i++];
        while ( !tdata.isNull() ) {
            t->addConnexionToCreate(make_pair(tdata["object"].asString(), tdata["cmd"].asString()));
            tdata = data["connexion"][i++];
        }
    }
    
    return t;
}

void Switch::resetBody(){
    if(_body)
        _world->DestroyBody(_body);
    
    b2BodyDef bd3;
    
    bd3.type          = b2_staticBody;
    bd3.fixedRotation = true;
    bd3.position.Set(_position.x, _position.y);
    
    _body = _world->CreateBody(&bd3);
    
    b2PolygonShape poly3;
    poly3.SetAsBox(_dim.x / 2, _dim.y / 2);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &poly3;
    fixtureDef.density = 0.5f;
    fixtureDef.friction = 0.9f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    _body->CreateFixture(&fixtureDef);
    
    for(b2Fixture *f = _body->GetFixtureList(); f != NULL; f = f->GetNext()){
        FixtureData *data = new FixtureData();
        data->switch_but = this;
        data->box_type   = BOX_TYPE_SWITCH;
        f->SetUserData((void*)data);
    }
}

void Switch::connect(Object *obj, string cmd){
    _connected.push_back(make_pair(obj, cmd));
    obj->destroySignal.connect(bind(&Switch::targetRemoved, this, _1));
    obj->handleCmd(cmd, _state);
}

void Switch::clicked(){
    for(vector<pair<Object*, string>>::iterator it = _connected.begin(); it != _connected.end(); ++it){
        it->first->handleCmd(it->second, _state);
    }
}

void Switch::targetRemoved(Object *target){
    vector<pair<Object*, string>> newLinks;
    for(vector<pair<Object*, string>>::iterator it = _connected.begin(); it != _connected.end(); ++it){
        if(it->first && it->first != target)
            newLinks.push_back(*it);
    }
    _connected = newLinks;
}

void Switch::draw(){
    if(_state)
        _img->setColor(0.0, 1.0, 0.0f);
    else
        _img->setColor(1.0, 0.0, 0.0);
    
    _img->setBasePosition(_position);
    _img->show();
    
    Object::draw();
}

void Switch::handleCmd(string cmd, bool state){
    if(cmd == "turn_on"){
        _state = state;
        clicked();
    }
}