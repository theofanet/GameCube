//
//  Window.hpp
//  TestMap
//
//  Created by Theo Fanet on 07/05/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include "String.h"
#include "Widget.h"
#include "Font.h"
#include "String.h"

namespace GRender { namespace UI {
    class Window : public GRender::UI::Widget{
    public:
        Window();
        ~Window();
        
        void setTitle(string t){ _title = t; }
        string getTitle(){ return _title; }
        
        void setPosition(GLfloat x, GLfloat y){ setPosition(GRPos(x, y)); }
        void setPosition(GRPos pos){ _position = pos; }
        GRPos getPosition(){ return _position; }
        
        void setSize(GLfloat width, GLfloat height){ setSize(GRSize(width, height)); }
        void setSize(GRSize size){ _size = size; }
        GRSize getSize(){ return _size; }
        
        void draw();
        
    private:
        string _title;
        GRPos  _position;
        GRSize _size;
        
        GRender::Font *_font;
    };
} }


#endif /* Window_hpp */
