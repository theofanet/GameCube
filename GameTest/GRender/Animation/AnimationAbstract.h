//
//  AnimationAbstract.hpp
//  TestMap
//
//  Created by Theo Fanet on 16/05/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef AnimationAbstract_hpp
#define AnimationAbstract_hpp

#include <stdio.h>
#include <iostream>
#include "time.h"

using namespace std;

namespace GRender {
    enum AnimationState { PLAYING, PAUSE };
    
    class AnimationAbstract;
    
    class AnimationDelegate{
    public:
        virtual void AnimationStarted(AnimationAbstract *a){};
        virtual void AnimationEnded(AnimationAbstract *a){};
        virtual void AnimationPaused(AnimationAbstract *a){};
    };

    class AnimationAbstract{
    public:
        AnimationAbstract(int from, int to, int delay);
        ~AnimationAbstract();
        
        virtual void pause();
        virtual void stop();
        virtual void start();
        virtual void play();
        
        virtual void reset(bool pause = false);
        virtual void update();
        virtual void redo(bool r);
        
        virtual void setStartIndex(int i);
        virtual int getIndex();
        
        void setRow(int row);
        int  getRow();
        
        void setDelegate(AnimationDelegate *d);
        
        void setIdentifier(string ident){ _identifier = ident; };
        string identifier(){ return _identifier; };
        
    protected:
        int _from, _to, _delay;
        
        int _index, _start_index;
        bool _redo;
        
        int _rowId;
        
        clock_t _last_time;
        string _identifier;
        
        AnimationState _current_state;
        AnimationDelegate *_delegate;
    };
}

#endif /* AnimationAbstract_hpp */
