//
//  MenuScene.hpp
//  GameCube
//
//  Created by Theo Fanet on 14/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef MenuScene_hpp
#define MenuScene_hpp

#include <stdio.h>
#include <dirent.h>

#include "GameScene.h"
#include "ParticleEmitter.h"

class MenuScene : public GameScene{
public:
    MenuScene();
    ~MenuScene();
    
    void middle_draw();
    void after_step();
    
    void listFolderFiles(string folder);
    
    void IoKeyboard(unsigned char key, IoKeyStats state);
    void IoKeyboardSpecial(int key, IoKeyStats state);
    
    string getSelectedMapPath();
    
    void setLoading(bool l){ _loading = l; }
    
protected:
    vector<string> _maps;
    
    ParticleEmitter *_emitter;
    
    bool _loading;
    
    int _selection;
    
    GRender::Font *_menuFont, *_titleFont, *_menuFontBlink;
};

#endif /* MenuScene_hpp */
