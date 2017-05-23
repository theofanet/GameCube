//
//  Widget.hpp
//  TestMap
//
//  Created by Theo Fanet on 07/05/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef Widget_hpp
#define Widget_hpp


#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>

#include <stdio.h>

#include "GUtils.h"
#include "defines.h"

namespace GRender { namespace UI {
    class Widget{
    public:
        Widget();
        ~Widget();
        
        virtual void draw(){}
        virtual void update(){}
    };
} }

#endif /* Widget_hpp */
