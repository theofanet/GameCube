//
//  CurlyAnimation.cpp
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "CurlyAnimation.h"

namespace GRender {
    
    CurlyAnimation::CurlyAnimation(int from, int to, int delay) : GRender::AnimationAbstract(from, to, delay){
        _step = 1;
    }
    
    void CurlyAnimation::update(){
        
        if(_current_state != AnimationState::PAUSE){
            _last_time++;
            
            if(_last_time > _delay){
                _last_time = 0;
                
                int sign = 1;
                if(_from > _to)
                    sign = -1;
                
                _index += _step*sign;
                
                if(_step > 0 && ((sign > 0 && _index >= _to) || (sign < 0 && _index <= _from)))
                    _step = -1;
                else if(_step < 0 && ((sign > 0 && _index <= _from) || (sign < 0 && _index >= _to))){
                    if(!_redo){
                        _index = _start_index;
                        stop();
                    }
                    
                    _step  = 1;
                }
                
            }
        }
        
    }
    
}