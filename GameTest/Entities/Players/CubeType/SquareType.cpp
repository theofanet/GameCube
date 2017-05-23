//
//  SquareType.cpp
//  GameTest2
//
//  Created by Theo Fanet on 07/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "SquareType.h"

SquareType::SquareType(Player *p, b2Vec2 dim) : PlayerType(p){
    _dim        = dim;
    _nbJump     = 0;
    _slowCount  = 0;
    _showDebug  = false;
    _isSlowdown = false;
    
    _rotation = ROTATE_NONE;
    
    _image = new GRender::Image("Resources/images/whiteRect.png");
    _image->setDrawSize(2 * _dim);
}

SquareType::~SquareType(){
    delete_obj(_image);
}

b2Body* SquareType::setGeometrie(b2World *world){
    b2Vec2 position = _cube->getBody() ? _cube->getBody()->GetPosition() : b2Vec2();
    
    if(_cube->getBody())
        world->DestroyBody(_cube->getBody());
    
    b2BodyDef bd3;
    
    bd3.type          = b2_dynamicBody;
    bd3.fixedRotation = false;
    bd3.position.Set(position.x, position.y);
    
    b2Body *body = world->CreateBody(&bd3);
    
    b2PolygonShape poly3;
    poly3.SetAsBox(_dim.x, _dim.y);
    body->CreateFixture(&poly3, 1.0);
    
    for(b2Fixture *f = body->GetFixtureList(); f != NULL; f = f->GetNext()){
        FixtureData *data = new FixtureData();
        data->player = _cube;
        data->box_type    = BOX_TYPE_CUBE_SQUARE;
        f->SetUserData((void*)data);
    }
    
    return body;
}

void SquareType::draw(){
    if(_image){
        _image->setBasePosition(_cube->getPosition().x, _cube->getPosition().y);
        _image->rotate(_cube->getBody()->GetAngle()*RADTODEG);
        _image->show();
    }
    else{
        glPushMatrix();
        glTranslatef(_cube->getBody()->GetPosition().x, _cube->getBody()->GetPosition().y, 0);
        glRotated(_cube->getBody()->GetAngle()*RADTODEG, 0, 0, 1);
        
        if(_jumping && _showDebug)
            glColor3f(1, 1, 0);
        else
            glColor3f(1, 1, 1);
        
        b2PolygonShape *shape = (b2PolygonShape *)_cube->getBody()->GetFixtureList()->GetShape();
        
        // Draw a textured quad
        glBegin(GL_QUADS);
        glVertex2f(shape->GetVertex(0).x, shape->GetVertex(0).y);
        glVertex2f(shape->GetVertex(1).x, shape->GetVertex(1).y);
        glVertex2f(shape->GetVertex(2).x, shape->GetVertex(2).y);
        glVertex2f(shape->GetVertex(3).x, shape->GetVertex(3).y);
        glEnd();
        
        glPopMatrix();
    }
}

void SquareType::update(){
    if(_rotation == ROTATE_RIGHT)
        _cube->getBody()->SetAngularVelocity(-ANGULAR_VELOCITY);
    else if(_rotation == ROTATE_LEFT)
        _cube->getBody()->SetAngularVelocity(ANGULAR_VELOCITY);
    else
        _cube->getBody()->SetAngularVelocity(0);
    
    if(_showDebug){
        _cube->getMap()->highlighTiles((int)(_cube->getBody()->GetPosition().x) / 2 - 1, (int)(_cube->getBody()->GetPosition().y / 2) - 1);
        _cube->getMap()->highlighTiles((int)(_cube->getBody()->GetPosition().x) / 2,     (int)(_cube->getBody()->GetPosition().y / 2) - 1);
        _cube->getMap()->highlighTiles((int)(_cube->getBody()->GetPosition().x) / 2 + 1, (int)(_cube->getBody()->GetPosition().y / 2) - 1);
        _cube->getMap()->highlighTiles((int)(_cube->getBody()->GetPosition().x) / 2 + 2, (int)(_cube->getBody()->GetPosition().y / 2) - 1);
        
        _cube->getMap()->highlighTiles((int)(_cube->getBody()->GetPosition().x) / 2 - 1, (int)(_cube->getBody()->GetPosition().y / 2));
        _cube->getMap()->highlighTiles((int)(_cube->getBody()->GetPosition().x) / 2 + 2, (int)(_cube->getBody()->GetPosition().y / 2));
        
        _cube->getMap()->highlighTiles((int)(_cube->getBody()->GetPosition().x) / 2 - 1, (int)(_cube->getBody()->GetPosition().y / 2) + 1);
        _cube->getMap()->highlighTiles((int)(_cube->getBody()->GetPosition().x) / 2 + 2, (int)(_cube->getBody()->GetPosition().y / 2) + 1);
        
        _cube->getMap()->highlighTiles((int)(_cube->getBody()->GetPosition().x) / 2 - 1, (int)(_cube->getBody()->GetPosition().y / 2) + 2);
        _cube->getMap()->highlighTiles((int)(_cube->getBody()->GetPosition().x) / 2,     (int)(_cube->getBody()->GetPosition().y / 2) + 2);
        _cube->getMap()->highlighTiles((int)(_cube->getBody()->GetPosition().x) / 2 + 1, (int)(_cube->getBody()->GetPosition().y / 2) + 2);
        _cube->getMap()->highlighTiles((int)(_cube->getBody()->GetPosition().x) / 2 + 2, (int)(_cube->getBody()->GetPosition().y / 2) + 2);
    }
    
    if(_isSlowdown){
        _slowCount++;
        if(_slowCount > SLOW_MAX_STEP){
            setUnactive();
        }
        else
            jetonUsedTimeUpdate.emit(_cube, _slowCount, SLOW_MAX_STEP);
    }
}

void SquareType::setUnactive(){
    _cube->getGameScene()->settings()->slow_down = 1;
    _isSlowdown = false;
    _slowCount = 0;
    
    if(_cube->getNbJetons() == 0)
        _cube->emptyJeton.emit(_cube);
    else
        _cube->reloadJeton.emit(_cube);
}

void SquareType::keyboardSpecial(int key, IoKeyStats state){
    if(state == IO_KEY_DOWN){
        if(key == GLUT_KEY_RIGHT)
            _rotation = ROTATE_RIGHT;
        else if(key == GLUT_KEY_LEFT)
            _rotation = ROTATE_LEFT;
        else if(key == GLUT_KEY_UP)
            jump();
    }
    else{
        if(key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT)
            _rotation = ROTATE_NONE;
    }
}

void SquareType::jump(){
    if(_nbJump < MAX_JUMP){
        _cube->getBody()->ApplyLinearImpulse(b2Vec2(_rotation*X_JUMP, Y_JUMP), _cube->getBody()->GetWorldCenter(), true);
        _jumping = true;
        _nbJump++;
    }
}

void SquareType::shoot(){
    vector<pair<int, int>> tile_pos;
    
    tile_pos.push_back(make_pair((int)(_cube->getBody()->GetPosition().x) / 2 - 1, (int)(_cube->getBody()->GetPosition().y / 2) - 1));
    
    tile_pos.push_back(make_pair((int)(_cube->getBody()->GetPosition().x) / 2, (int)(_cube->getBody()->GetPosition().y / 2) - 1));
    tile_pos.push_back(make_pair((int)(_cube->getBody()->GetPosition().x) / 2 + 1, (int)(_cube->getBody()->GetPosition().y / 2) - 1));
    
    tile_pos.push_back(make_pair((int)(_cube->getBody()->GetPosition().x) / 2 + 2, (int)(_cube->getBody()->GetPosition().y / 2) - 1));
    
    tile_pos.push_back(make_pair((int)(_cube->getBody()->GetPosition().x) / 2 - 1, (int)(_cube->getBody()->GetPosition().y / 2)));
    tile_pos.push_back(make_pair((int)(_cube->getBody()->GetPosition().x) / 2 + 2, (int)(_cube->getBody()->GetPosition().y / 2)));
    
    tile_pos.push_back(make_pair((int)(_cube->getBody()->GetPosition().x) / 2 - 1, (int)(_cube->getBody()->GetPosition().y / 2) + 1));
    tile_pos.push_back(make_pair((int)(_cube->getBody()->GetPosition().x) / 2 + 2, (int)(_cube->getBody()->GetPosition().y / 2) + 1));
    
    tile_pos.push_back(make_pair((int)(_cube->getBody()->GetPosition().x) / 2 - 1, (int)(_cube->getBody()->GetPosition().y / 2) + 2));
    tile_pos.push_back(make_pair((int)(_cube->getBody()->GetPosition().x) / 2,     (int)(_cube->getBody()->GetPosition().y / 2) + 2));
    tile_pos.push_back(make_pair((int)(_cube->getBody()->GetPosition().x) / 2 + 1, (int)(_cube->getBody()->GetPosition().y / 2) + 2));
    tile_pos.push_back(make_pair((int)(_cube->getBody()->GetPosition().x) / 2 + 2, (int)(_cube->getBody()->GetPosition().y / 2) + 2));
    
    for(size_t i = 0; i < tile_pos.size(); i++){
        int x = tile_pos.at(i).first * 2;
        int y = tile_pos.at(i).second * 2;
        
        MapTile *t = _cube->getMap()->getTile(x, y);
        if(t && t->isDestructible())
            t->destroy();
    }
}

void SquareType::endJump(){
    PlayerType::endJump();
    _nbJump = 0;
}

void SquareType::keyboard(unsigned char key, IoKeyStats state){
    if(state == IO_KEY_DOWN){
        if(key == GLUT_KEY_SPACE)
            jump();
        else if(key == 'z')
            shoot();
        else if(key == 's' && !_isSlowdown && _cube->getNbJetons() > 0){
            _cube->getGameScene()->settings()->slow_down = SLOW_LEVEL;
            _isSlowdown = true;
            _cube->removeJeton();
        }
        else if(key == 'p')
            _showDebug = !_showDebug;
    }
}
