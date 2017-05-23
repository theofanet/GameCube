//
//  CircleType.cpp
//  GameTest2
//
//  Created by Theo Fanet on 08/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "CircleType.h"

CircleType::CircleType(Player *p, GLfloat radius) : PlayerType(p){
    _radius   = radius;
    _isLittle = false;
    _nbJump   = 0;
    _rotation = ROTATE_NONE;
    
    _image = new GRender::Image("Resources/images/whiteCircle.png");
}

CircleType::~CircleType(){
    delete_obj(_image);
}

b2Body* CircleType::setGeometrie(b2World *world){
    b2Vec2 position = _cube->getBody() ? _cube->getBody()->GetPosition() : b2Vec2();
    
    if(_cube->getBody())
        world->DestroyBody(_cube->getBody());
    
    b2BodyDef bd3;
    
    bd3.type          = b2_dynamicBody;
    bd3.fixedRotation = false;
    bd3.position.Set(position.x, position.y);
    
    b2Body *body = world->CreateBody(&bd3);
    
    b2CircleShape sphere3;
    sphere3.m_p.Set(0, 0); //position, relative to body position
    sphere3.m_radius = _radius;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &sphere3;
    fixtureDef.density = 0.5f;
    fixtureDef.friction = 0.4f;
    fixtureDef.restitution = 0.3f; // Make it bounce a little bit
    body->CreateFixture(&fixtureDef);
    
    for(b2Fixture *f = body->GetFixtureList(); f != NULL; f = f->GetNext()){
        FixtureData *data = new FixtureData();
        data->player = _cube;
        data->box_type    = BOX_TYPE_CUBE_CIRCLE;
        f->SetUserData((void*)data);
    }
    
    return body;
}

void CircleType::draw(){
    if(_image){
        _image->setBasePosition(_cube->getPosition().x, _cube->getPosition().y);
        _image->setDrawSize(2 * b2Vec2(_radius, _radius));
        _image->rotate(_cube->getBody()->GetAngle()*RADTODEG);
        _image->show();
    }
    else{
        glPushMatrix();
        glTranslatef(_cube->getBody()->GetPosition().x, _cube->getBody()->GetPosition().y, 0);
        glRotated(_cube->getBody()->GetAngle()*RADTODEG, 0, 0, 1);
        
        glColor3f(1, 1, 1);
        
        const float32 k_segments = 16.0f;
        const float32 k_increment = 2.0f * b2_pi / k_segments;
        float32 theta = 0.0f;
        glBegin(GL_POLYGON);
        for (int32 i = 0; i < k_segments; ++i)
        {
            b2Vec2 v = _radius * b2Vec2(cosf(theta), sinf(theta));
            glVertex2f(v.x, v.y);
            theta += k_increment;
        }
        glEnd();
        
        glPopMatrix();
    }
}

void CircleType::setUnactive(){
    _littleCount = 0;
    if(_isLittle){
        _radius  *= 2.0;
        _cube->_body = setGeometrie(_cube->_world);
    }
    _isLittle = false;
    
    if(_cube->getNbJetons() == 0)
        _cube->emptyJeton.emit(_cube);
    else
        _cube->reloadJeton.emit(_cube);
}


void CircleType::update(){
    if(_rotation == ROTATE_RIGHT)
        _cube->getBody()->SetAngularVelocity(-C_ANGULAR_VELOCITY);
    else if(_rotation == ROTATE_LEFT)
        _cube->getBody()->SetAngularVelocity(C_ANGULAR_VELOCITY);
    else
        _cube->getBody()->SetAngularVelocity(0);
    
    if(_isLittle){
        _littleCount++;
        
        if(_littleCount > LITTLE_MAX_STEP){
            setUnactive();
        }
        else
            jetonUsedTimeUpdate.emit(_cube, _littleCount, LITTLE_MAX_STEP);
    }
}

void CircleType::keyboardSpecial(int key, IoKeyStats state){
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

void CircleType::jump(){
    if(_nbJump < 2){
        _cube->getBody()->ApplyLinearImpulse(b2Vec2(_rotation*(_isLittle ? X_JUMP_LITTLE : X_JUMP_BIG), _isLittle ? Y_JUMP_LITTLE : Y_JUMP_BIG), _cube->getBody()->GetWorldCenter(), true);
        _jumping = true;
        _nbJump++;
    }
}

void CircleType::endJump(){
    PlayerType::endJump();
    _nbJump = 0;
}

void CircleType::keyboard(unsigned char key, IoKeyStats state){
    if(state == IO_KEY_DOWN){
        if(key == GLUT_KEY_SPACE)
            jump();
        else if(key == 's'){
            if(!_isLittle && _cube->getNbJetons() > 0){
                _isLittle = true;
                _radius  *= 0.5;
                _cube->_body = setGeometrie(_cube->_world);
                _cube->removeJeton();
            }
        }
        else if(key == 'd'){
            b2Vec2 pos = _cube->getPosition();
            if(_rotation == ROTATE_LEFT)
                pos.x -= 5;
            else if(_rotation == ROTATE_RIGHT)
                pos.x += 5;
            _cube->setPosition(pos);
        }
    }
}
