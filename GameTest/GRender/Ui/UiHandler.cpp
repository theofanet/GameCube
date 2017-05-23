//
//  Ui.cpp
//  TestMap
//
//  Created by Theo Fanet on 08/05/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "UiHandler.h"


namespace GRender { namespace UI {
   
    UiHandler::UiHandler(){
        
    }
    
    UiHandler::~UiHandler(){
        for(size_t i = 0; i < _widgets.size(); i++)
            delete_obj(_widgets.at(i));
    }
    
    void UiHandler::IoMouseClick(b2Vec2 position, IoMouseButtons button, IoKeyStats state){
        cout << "Clicked coordinates : " << position.x << " " << position.y << endl;
    }
    
    void UiHandler::IoMouseMove(b2Vec2 p, bool passive){
        
    }
    
    void UiHandler::IoKeyboard(unsigned char key, IoKeyStats state){
        
    }
    
    void UiHandler::IoKeyboardSpecial(int key, IoKeyStats state){
        
    }
    
    void UiHandler::draw(){
        for(size_t i = 0; i < _widgets.size(); i++)
            _widgets.at(i)->draw();
    }
    
} }