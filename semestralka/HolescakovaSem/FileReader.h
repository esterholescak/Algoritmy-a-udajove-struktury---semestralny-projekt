#pragma once
#include "StopData.h"
#include <vector>  
#include <string>    

class FileReader {
public:
    static std::vector<StopData> loadFromFile(const std::string& filename);
};