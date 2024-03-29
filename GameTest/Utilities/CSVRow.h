//
//  CSVRow.hpp
//  TestMap
//
//  Created by Theo Fanet on 07/05/16.
//  Copyright © 2016 Theo. All rights reserved.
//

#ifndef CSVRow_hpp
#define CSVRow_hpp

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class CSVRow{
public:
    std::string const& operator[](std::size_t index) const{
        return at(index);
    }
    
    std::string const& at(std::size_t index) const{
        return m_data[index];
    }
    
    std::size_t size() const{
        return m_data.size();
    }
    
    void readNextRow(std::istream& str){
        std::string line;
        std::getline(str,line);
        
        std::stringstream lineStream(line);
        std::string cell;
        
        m_data.clear();
        while(std::getline(lineStream, cell, ','))
            m_data.push_back(cell);
    }
    
private:
    std::vector<std::string> m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data);

#endif /* CSVRow_hpp */
