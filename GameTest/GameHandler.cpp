//
//  GameHandler.cpp
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "GameHandler.h"

GameHandler::GameHandler(){
    //OPENGL INIT
    glClearColor(.2, .2, .2, .5);
    glShadeModel(GL_FLAT);
    glAlphaFunc(GL_GREATER, 0); //For alpha
    glEnable(GL_ALPHA_TEST);  //Same
    glEnable(GL_BLEND); //Same
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Same
    
    //VARS INIT
    _fps          = 0;
    _frameCount   = 0;
    _currentTime  = 0;
    _previousTime = 0;
    
    _width  = WIDTH;
    _height = HEIGHT;
    
    _current_scene_index = 0;
    _to_remove           = NULL;

    _Io = new IoHandler();
    
    _menuScene = new MenuScene();
    _menuScene->scenes_handler = this;
    
    setCurrentScene(_menuScene);
}

GameHandler::~GameHandler(){
    for(size_t i = 0; i < _scenes.size(); i++)
        delete_obj(_scenes.at(i));
    delete_obj(_menuScene);
    delete_obj(_Io);
}

void GameHandler::setCurrentScene(GameScene *s){
    s->load_world();
    
    _current_scene = s;
    reshape(_width, _height);
    
    if(_Io->delegate() != s)
        _Io->setDelegate(s);
}

void GameHandler::run(){
    //Lancement boucle
    glutMainLoop();
}

void GameHandler::quit_action(int code){
    #ifdef FREEGLUT
    glutLeaveMainLoop();
    #endif
    
    exit(code);
}

b2Vec2 GameHandler::ConvertScreenToWorld(int x, int y){
    float u = x / float(_width);
    float v = (_height - y) / float(_height);
    
    float ratio = float(_width) / float(_height);
    b2Vec2 extents(ratio * _current_scene->settings()->viewSize, _current_scene->settings()->viewSize);
    extents *= _current_scene->settings()->viewZoom;
    
    b2Vec2 lower = _current_scene->settings()->viewCenter - extents;
    b2Vec2 upper = _current_scene->settings()->viewCenter + extents;
    
    b2Vec2 p;
    p.x = (1.0f - u) * lower.x + u * upper.x;
    p.y = (1.0f - v) * lower.y + v * upper.y;
    
    return p;
}

void GameHandler::reshape(int w, int h){
    _width  = w;
    _height = h;
    
    glViewport(0, 0, _width, _height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = float(_width) / float(_height);
    
    b2Vec2 extents(ratio * _current_scene->settings()->viewSize, _current_scene->settings()->viewSize);
    extents *= _current_scene->settings()->viewZoom;
    
    b2Vec2 lower = _current_scene->settings()->viewCenter - extents;
    b2Vec2 upper = _current_scene->settings()->viewCenter + extents;
    
    _current_scene->settings()->lower  = lower;
    _current_scene->settings()->upper  = upper;
    _current_scene->settings()->height = _height;
    _current_scene->settings()->width  = _width;
    
    gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
}

void GameHandler::display(void){
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    _current_scene->draw_scene();
    
    glutSwapBuffers();
}

void GameHandler::idle(void){
    int elapse_t = handleFPS();
    
    _current_scene->Step(elapse_t);
    
    if(_current_scene->exitApp())
        quit_action();
    
    if(_to_remove)
        delete_obj(_to_remove);
    
    glutPostRedisplay();
}

int GameHandler::handleFPS(){
    _frameCount++;
    
    _currentTime = glutGet(GLUT_ELAPSED_TIME);
    int timeInterval = _currentTime - _previousTime;
    
    while(timeInterval < 1000/FPS){
        _currentTime = glutGet(GLUT_ELAPSED_TIME);
        timeInterval = _currentTime - _previousTime;
    }
    
    _fps          = _frameCount / (timeInterval / 1000.0f);
    _previousTime = _currentTime;
    _frameCount   = 0;
    
    char *TempString = (char*)malloc(512 + strlen(WINDOW_TITLE));
    sprintf(TempString,"%s - %4.2f FPS", WINDOW_TITLE, _fps);
    glutSetWindowTitle(TempString);
    free(TempString);
    
    return timeInterval;
}

void GameHandler::keyboard(unsigned char key, int x, int y){
    _Io->KeyBoard(key);
}

void GameHandler::keyboard_up(unsigned char key, int x, int y){
    _Io->KeyBoardUp(key);
}

void GameHandler::specialKeyboard_up(int key, int x, int y){
    _Io->KeyBoardSpecialUp(key);
}

void GameHandler::specialKeyboard(int key, int x, int y){
    _Io->KeyBoardSpecial(key);
}

void GameHandler::mouse_click(int button, int state, int x, int y){
    b2Vec2 p = ConvertScreenToWorld(x, y);
    _Io->MouseClick(p, button, state);
}

void GameHandler::mouse_motion(int x, int y){
    b2Vec2 p = ConvertScreenToWorld(x, y);
    _Io->MouseMove(p);
}

void GameHandler::mouse_passive_motion(int x, int y){
    b2Vec2 p = ConvertScreenToWorld(x, y);
    _Io->MouseMove(p, true);
}

void GameHandler::goToScene(GameScene *fromScene, GameScene *toScene){
    setCurrentScene(toScene);
}

void GameHandler::goNextScene(GameScene *fromScene){
    if(fromScene == _menuScene){
        display();
        CubeWord *lvl = new CubeWord(_menuScene->getSelectedMapPath());
        lvl->scenes_handler = this;
        setCurrentScene(lvl);
    }
    else{
        _to_remove = fromScene;
        _menuScene->setLoading(false);
        setCurrentScene(_menuScene);
    }
}

void GameHandler::exitScene(GameScene *fromScene){
    quit_action();
}