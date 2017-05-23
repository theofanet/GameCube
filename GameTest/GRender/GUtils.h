//
//  GUtils.h
//  GameTest2
//
//  Created by Theo Fanet on 11/10/15.
//  Copyright © 2015 Theo. All rights reserved.
//

#ifndef GUtils_h
#define GUtils_h


#include <iostream>

namespace GRender {
    
    struct GRColor{
        GRColor(){
            set();
        }
        
        GRColor(float cr, float cg = 1.0f, float cb = 1.0f){
            set(cr, cg, cb);
        }
        
        GRColor(int cr, int cg = 255, int cb = 255){
            set(cr, cg, cb);
        }
        
        
        void set(float cr = 1.0f, float cg = 1.0f, float cb = 1.0f){
            r = cr;
            g = cg;
            b = cb;
        }
        
        void set(int cr, int cg = 255, int cb = 255){
            r = (float)cr/255;
            g = (float)cg/255;
            b = (float)cb/255;
        }
        
        
        float r, g, b;
    };
    
    struct GRPos{
        GRPos(){
            x = 0;
            y = 0;
        }
        
        GRPos(GLfloat x, GLfloat y){
            x = x;
            y = y;
        }
        
        GLfloat x, y;
    };
    
    struct GRSize{
        GRSize(){
            w = 0;
            h = 0;
        }
        
        GRSize(GLfloat sw, GLfloat sh){
            w = sw;
            h = sh;
        }
        
        GLfloat w, h;
    };
    
}

#endif /* GUtils_h */
