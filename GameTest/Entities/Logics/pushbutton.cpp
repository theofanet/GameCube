#include "pushbutton.h"

PushButton::PushButton(b2World *world, string name, b2Vec2 pos) : Object(world, name, pos){
    _type = OBJECT_TYPE_PUSH_BUTTON;
    
    _img = new GRender::Image("Resources/Images/Objects/PushButton.png");
    _img->setDrawSize(b2Vec2(2, 2));
    
    _cmdList.push_back("push");
    resetBody();
}

PushButton::~PushButton(){
    delete_obj(_img);
}

Json::Value PushButton::getJsonData(){
    Json::Value data = Object::getJsonData();
    
    int i = 0;
    for(vector<pair<Object*, string>>::iterator it = _connected.begin(); it != _connected.end(); ++it){
        data["connexion"][i]["object"] = it->first->getName();
        data["connexion"][i]["cmd"]    = it->second;
        i++;
    }
    
    return data;
}

PushButton* PushButton::fromJsonData(b2World *world, Json::Value data, int index, string){
    string name = StringFormatter::format("switch_%i", index);
    if(data.isMember("name"))
        name = data["name"].asString();
    
    PushButton *t = new PushButton(world, name, b2Vec2(data["position"]["x"].asDouble(), data["position"]["y"].asDouble()));
    
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

void PushButton::resetBody(){
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
        data->push_but = this;
        data->box_type = BOX_TYPE_PUSH_BUTTON;
        f->SetUserData((void*)data);
    }
}

void PushButton::connect(Object *obj, string cmd){
    _connected.push_back(make_pair(obj, cmd));
    obj->destroySignal.connect(bind(&PushButton::targetRemoved, this, _1));
}

void PushButton::push(){
    for(vector<pair<Object*, string>>::iterator it = _connected.begin(); it != _connected.end(); ++it)
        it->first->handleCmd(it->second, 1);
}

void PushButton::targetRemoved(Object *target){
    vector<pair<Object*, string>> newLinks;
    for(vector<pair<Object*, string>>::iterator it = _connected.begin(); it != _connected.end(); ++it){
        if(it->first && it->first != target)
            newLinks.push_back(*it);
    }
    _connected = newLinks;
}

void PushButton::draw(){
    _img->setColor(1.0, 1.0, 1.0);
    _img->setBasePosition(_position);
    _img->show();
    
    Object::draw();
}

void PushButton::handleCmd(string cmd, bool){
    if(cmd == "push"){
        push();
    }
}