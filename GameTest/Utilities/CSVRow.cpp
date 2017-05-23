//
//  CSVRow.cpp
//  TestMap
//
//  Created by Theo Fanet on 07/05/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#include "CSVRow.h"

std::istream& operator>>(std::istream& str, CSVRow& data){
    data.readNextRow(str);
    return str;
}