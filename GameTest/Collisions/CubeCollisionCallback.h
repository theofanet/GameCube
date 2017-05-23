//
//  CubeCollisionCallback.hpp
//  GameTest2
//
//  Created by Theo Fanet on 11/07/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef CubeCollisionCallback_hpp
#define CubeCollisionCallback_hpp

#include <iostream>
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#include "Box2D.h"

#include "defines.h"
#include "SOIL2.h"
#include "string_helper.h"

#include "json.h"
#include "b2dJson.h"

#include "Objects.h"

#include "GRender.h"
#include "CubePlayer.h"

class GameScene;

class CubeCollisionCallback : public b2ContactListener
{
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
};

#endif /* CubeCollisionCallback_hpp */
