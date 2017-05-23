//
//  Ui.hpp
//  TestMap
//
//  Created by Theo Fanet on 08/05/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef Ui_hpp
#define Ui_hpp

#include <stdio.h>
#include <vector>

#include "IoHandler.h"
#include "defines.h"
#include "Widget.h"

namespace GRender{ namespace UI {
    
    class UiHandler : public IoHandlerDelegate{
    public:
        UiHandler();
        ~UiHandler();
        
        void IoMouseClick(b2Vec2 position, IoMouseButtons button, IoKeyStats state);
        void IoMouseMove(b2Vec2 p, bool passive);
        void IoKeyboard(unsigned char key, IoKeyStats state);
        void IoKeyboardSpecial(int key, IoKeyStats state);
        
        void addWidget(Widget *w){ _widgets.push_back(w); }
        
        void draw();
        
    private:
        vector<Widget *> _widgets;
        Widget *_focusedWidget;
    };
    
} }

#endif /* Ui_hpp */
