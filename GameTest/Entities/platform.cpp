#include "platform.h"

Platform::Platform(b2World *world, string name, b2Vec2 pos) : Object(world, name, pos){
    _isMoving = false;
    
    _type = OBJECT_TYPE_PLATFORM;
    _dim  = b2Vec2(10.0, 2.0);
    
    _nextIndex = 0;
    
    _actualPosition = _position;
    
    _actualDirection    = true;
    _oneStepMove        = false;
    _normalizeDirection = true;
    _movingCircle       = false;
    _speed              = 5.0f;
    
    _texture = NULL;
    _showTexture = false;
    
    _cmdList.push_back("power");
    _cmdList.push_back("one_step_front");
    _cmdList.push_back("one_step_back");
    _cmdList.push_back("setDirection");
    _cmdList.push_back("toggleDirection");
    _cmdList.push_back("resetPosition");
    
    _maxPoint = _position;
    _minPoint = _position;
    
    _footFixture = NULL;
    _body = NULL;
    resetBody();
}

Platform::~Platform(){
    delete_obj(_texture);
}

Json::Value Platform::getJsonData(){
    Json::Value data = Object::getJsonData();
    
    data["isMoving"] = _isMoving;
    data["movingCircle"] = _movingCircle;
    data["direction"] = _actualDirection;
    data["normalizeDirection"] = _normalizeDirection;
    data["speed"] = _speed;
    data["size"]["x"] = _dim.x;
    data["size"]["y"] = _dim.y;
    data["showTexture"] = _showTexture;
    data["textureFilename"] = "";
    
    int i = 0;
    for(vector<b2Vec2>::iterator it = _points.begin(); it != _points.end(); ++it){
        b2Vec2 pts = *it;
        data["points"][i]["x"] = pts.x;
        data["points"][i]["y"] = pts.y;
        i++;
    }
    
    return data;
}

Platform* Platform::fromJsonData(b2World *world, Json::Value data, int index, string basefilepath){
    string name = StringFormatter::format("laser_%i", index);
    if(data.isMember("name"))
        name = data["name"].asString();
    
    Platform *t = new Platform(world, name, b2Vec2(data["position"]["x"].asDouble(), data["position"]["y"].asDouble()));
    
    t->setSize(data["size"]["x"].asDouble(), data["size"]["y"].asDouble());
    if(data.isMember("showTexture")){
        t->showTexture(data["showTexture"].asBool());
        if(t->isShowingTexture())
            t->setTexture(basefilepath + data["textureFilename"].asString());
    }
    
    if(data.isMember("points")){
        int i = 0;
        Json::Value tdata = data["points"][i++];
        while ( !tdata.isNull() ) {
            t->addPoint(tdata["x"].asDouble(), tdata["y"].asDouble());
            tdata = data["points"][i++];
        }
    }
    
    t->setMovingCircle(data["movingCircle"].asBool());
    t->setDirection(data["direction"].asBool());
    t->setNormalizeDirection(data["normalizeDirection"].asBool());
    t->setSpeed(data["speed"].asDouble());
    t->setIsMoving(data["isMoving"].asBool());
    
    return t;
}

void Platform::resetBody(){
    if(_body)
        _world->DestroyBody(_body);
    
    b2BodyDef bd3;
    
    bd3.type          = b2_kinematicBody;
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
    _body->CreateFixture(&fixtureDef);
    
    b2PolygonShape footPoly;
    footPoly.SetAsBox(_dim.x / 2 - 0.5, _dim.y / 2 - 0.5, b2Vec2(0, 1), 0);
    b2FixtureDef footFixtureDef;
    footFixtureDef.shape       = &footPoly;
    footFixtureDef.density     = 0.5f;
    footFixtureDef.friction    = 0.9f;
    footFixtureDef.restitution = 0.0f;
    footFixtureDef.isSensor    = true;
    _footFixture = _body->CreateFixture(&footFixtureDef);
    
    for(b2Fixture *f = _body->GetFixtureList(); f != NULL; f = f->GetNext()){
        FixtureData *data = new FixtureData();
        data->platform = this;
        data->box_type = BOX_TYPE_PLATFORM;
        f->SetUserData((void*)data);
    }
}

void Platform::setTexture(string filename){
    if(!filename.empty()){
        if(_texture)
            delete_obj(_texture);
        
        _texture = new GRender::Image(filename);
        _texture->setDrawSize(_dim);
        _filename = filename;
    }
}

void Platform::updateVelocity(){
    if(_isMoving && _points.size()){
        b2Vec2 pos = _position;
        b2Vec2 nextPos;
        if(_nextIndex == -1)
            nextPos = _actualPosition;
        else
            nextPos = _points.at(_nextIndex);
        
        b2Vec2  dir  = nextPos - pos;
        if(_normalizeDirection)
            dir.Normalize();
        
        if(_body){
            setPosition(_position);
            _body->SetLinearVelocity((_speed / 6.0) * dir);
        }
    }
    else if(_body){
        _body->SetLinearVelocity(b2Vec2(0, 0));
    }
}

void Platform::handleCmd(string cmd, bool state){
    if(cmd == "power")
        setIsMoving(state);
    else if(cmd == "setDirection")
        _actualDirection = state;
    else if(cmd == "toggleDirection")
        _actualDirection = !_actualDirection;
    else if(cmd == "one_step_front"){
        _oneStepMove = true;
        _isMoving    = true;
        updateVelocity();
    }
    else if(cmd == "resetPosition"){
        setPosition(_actualPosition);
        _nextIndex = (_actualDirection ? 0 : (int)_points.size() - 1);
        updateVelocity();
    }
    else if(cmd == "one_step_back"){
        _nextIndex -= 2;
        if(_nextIndex <= -2){
            if(_movingCircle)
                _nextIndex = (_actualDirection ? (int)_points.size() - 1 : -1);
            else{
                _actualDirection = !_actualDirection;
                _nextIndex = (_actualDirection ? (int)_points.size() - 1 : 0);
            }
        }
        _oneStepMove = true;
        _isMoving    = true;
    }
}

void Platform::setIsMoving(bool b){
    _isMoving = b;
    updateVelocity();
}

void Platform::draw(){
    
    b2Vec2 pos = _position;
    
    if(_showTexture && _texture){
        _texture->setDrawSize(_dim);
        _texture->setBasePosition(pos);
        _texture->setColor(1.0, 1.0, 1.0, 1.0);
        _texture->show();
    }
    else{
        glPushMatrix();
        glTranslated(pos.x, pos.y, 0.0f);
        
        glColor4f(1.0, 1.0, 1.0, 1.0);
        glBegin(GL_QUADS);
        glVertex2f(-_dim.x/2, -_dim.y/2);
        glVertex2f(-_dim.x/2, _dim.y/2);
        glVertex2f(_dim.x/2, _dim.y/2);
        glVertex2f(_dim.x/2, -_dim.y/2);
        glEnd();
        
        glPopMatrix();
    }
    
    /*glPushMatrix();
    
    BBox box = getRectBox();
    
    glColor4f(1.0, 1.0, 0.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(box.x[0], box.y[0]);
    glVertex2f(box.x[1], box.y[1]);
    glVertex2f(box.x[2], box.y[2]);
    glVertex2f(box.x[3], box.y[3]);
    glEnd();
    
    glPopMatrix();*/
    
    Object::draw();
}

void Platform::update(GLfloat dt){
    if(_body)
        _position = _body->GetPosition();
    
    if(_isMoving && _points.size()){
        b2Vec2 pos = getPosition();
        b2Vec2 nextPos;
        if(_nextIndex == -1)
            nextPos = _actualPosition;
        else
            nextPos = _points.at(_nextIndex);
        
        GLfloat dist = sqrt(pow(pos.x-nextPos.x, 2) + pow(pos.y-nextPos.y, 2));
        if(dist <= 0.1){
            _nextIndex += (_actualDirection ? 1 : -1);
            _position = nextPos;
            
            if(_nextIndex == _points.size() || _nextIndex <= -2){
                if(_movingCircle){
                    _nextIndex = (_actualDirection ? -1 : (int)_points.size() - 1);
                }
                else{
                    _actualDirection = !_actualDirection;
                    _nextIndex = (_actualDirection ? 0 : (int)_points.size() - 1);
                }
            }
            
            if(_oneStepMove){
                _oneStepMove = false;
                _isMoving    = false;
            }
            
            updateVelocity();
        }
    }
}

void Platform::addPoint(b2Vec2 p){
    _points.push_back(p);
    
    if(p.x < _minPoint.x)
        _minPoint.x = p.x;
    if(p.x > _maxPoint.x)
        _maxPoint.x = p.x;
    
    if(p.y < _minPoint.y)
        _minPoint.y = p.y;
    if(p.y > _maxPoint.y)
        _maxPoint.y = p.y;
}

BBox Platform::getRectBox(){
    Rect rect;
    
    rect.min.x = _minPoint.x - _dim.x / 2;
    rect.min.y = _minPoint.y - _dim.y / 2;
    rect.max.x = _maxPoint.x + _dim.x / 2;
    rect.max.y = _maxPoint.y + _dim.y / 2;
    
    BBox box(rect);
    return box;
}

