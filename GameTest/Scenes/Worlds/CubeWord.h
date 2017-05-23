//
//  CubeWord.hpp
//  GameTest2
//
//  Created by Theo Fanet on 19/06/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef CubeWord_hpp
#define CubeWord_hpp

#include <stdio.h>

#include "mapjsondata.h"
#include "Map.h"

#include "GameScene.h"
#include "CubePlayer.h"

#include "ProgressBar.h"

#include "CubeCollisionCallback.h"

class CubeWord : public GameScene, CheckPointDelegate{
public:
    CubeWord(string map_filename);
    ~CubeWord();
    
    void load_world();
    void after_step();
    void middle_draw();
    
    void back_draw();
    void front_draw();    
    
    void IoKeyboard(unsigned char key, IoKeyStats state);
    void IoKeyboardSpecial(int key, IoKeyStats state);
    
    void CheckPointTriggered(CheckPoint *checkpoint);
    
    void toggleGeo();
    
public slots:
    void NoMoreJetonSlot(Player *cube);
    void JetonReloadSlot(Player *cube);
    void JetonTimeUpdateSlot(Player *cube, int time, int max_time);

private:
    Map *_map;
    
    CubePlayer *_cube;
    
    float _wantedZoom;
    
    GLfloat _startTime, _finalTime;
    bool    _sceneFinish;
    
    GRender::Font *_menuFont;
    
    ProgressBar *_progress;
};


#endif /* CubeWord_hpp */
