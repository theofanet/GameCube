//
//  string_helper.cpp
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "string_helper.h"

string StringFormatter::format(const char *format, ...){
    size_t  nSize     = 1024;
    char    *lpBuffer = (char*)malloc(nSize);
    
    va_list lpParams;
    
    while (true){
        va_start(lpParams, format);
        int nResult = vsnprintf(lpBuffer, nSize, format, lpParams);
        va_end(lpParams);
        
        if ((nResult >= 0) && (nResult < (int)nSize)){
            lpBuffer[nResult] = '\0';
            std::string sResult(lpBuffer);
            free(lpBuffer);
            
            return sResult;
        }
        else{
            nSize = (nResult < 0) ? nSize *= 2 : (nResult + 1);
            lpBuffer = (char *)realloc(lpBuffer, nSize);
        }
    }
}