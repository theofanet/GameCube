#include "anddoor.h"

AndDoor::AndDoor(b2World *world, string name, b2Vec2 pos) : Object(world, name, pos){
    _type = OBJECT_TYPE_AND_DOOR;
    
    _entries[0] = false;
    _entries[1] = false;
    _out = false;
    
    _cmdList.push_back("entry_a");
    _cmdList.push_back("entry_b");
}

AndDoor::~AndDoor(){
    
}

Json::Value AndDoor::getJsonData(){
    Json::Value data = Object::getJsonData();
    
    data["entry_a"] = _entries[0];
    data["entry_b"] = _entries[1];
    int i = 0;
    for(vector<pair<Object*, string>>::iterator it = _connected.begin(); it != _connected.end(); ++it){
        data["connexion"][i]["object"] = it->first->getName();
        data["connexion"][i]["cmd"]    = it->second;
        i++;
    }
    
    return data;
}

AndDoor* AndDoor::fromJsonData(b2World *world, Json::Value data, int index, string){
    string name = StringFormatter::format("anddoor_%i", index);
    if(data.isMember("name"))
        name = data["name"].asString();
    
    AndDoor *t = new AndDoor(world, name, b2Vec2(data["position"]["x"].asDouble(), data["position"]["y"].asDouble()));
    
    t->setEntryA(data["entry_a"].asBool());
    t->setEntryB(data["entry_b"].asBool());
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

void AndDoor::setEntry(int index, bool b){
    if(index == 0 || index == 1){
        _entries[index] = b;
        bool oldOut = _out;
        _out = _entries[0] && _entries[1];
        if(oldOut != _out){
            notifyConnexions();
            outChanged.emit(_out);
        }
    }
}

void AndDoor::connect(Object *obj, string cmd){
    _connected.push_back(make_pair(obj, cmd));
    obj->destroySignal.connect(bind(&AndDoor::targetRemoved, this, _1));
    obj->handleCmd(cmd, _out);
}

void AndDoor::targetRemoved(Object *target){
    vector<pair<Object*, string>> newLinks;
    for(vector<pair<Object*, string>>::iterator it = _connected.begin(); it != _connected.end(); ++it){
        if(it->first && it->first != target)
            newLinks.push_back(*it);
    }
    _connected = newLinks;
}

void AndDoor::notifyConnexions(){
    for(vector<pair<Object*, string>>::iterator it = _connected.begin(); it != _connected.end(); ++it)
        it->first->handleCmd(it->second, _out);
}

void AndDoor::handleCmd(string cmd, bool state){
    if(cmd == "entry_a")
        setEntryA(state);
    else if(cmd == "entry_b")
        setEntryB(state);
}

void AndDoor::draw(){
    glPushMatrix();
    glTranslated(_position.x, _position.y, 0.0f);
    glLineWidth(5.0);
    if(_out)
        glColor4f(0.0, 1.0, 0.0, 1.0);
    else
        glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
    glLineWidth(1.0);
    glPopMatrix();
    
    Object::draw();
}