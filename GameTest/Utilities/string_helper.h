//
//  string_helper.h
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#ifndef __GameTest__string_helper__
#define __GameTest__string_helper__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <string>

using namespace std;

class StringFormatter
{
public:
    static string format(const char *format, ...);
};

#endif /* defined(__GameTest__string_helper__) */
