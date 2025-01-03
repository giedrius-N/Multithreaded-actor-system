#ifndef UTILS_HPP
#define UTILS_HPP

#include "CityTypes.hpp"
#include <iostream>
#include <fstream>
#include <vector>

class Utils
{
private:
    Utils() = delete;
    Utils(const Utils&) = delete;
    Utils& operator=(const Utils&) = delete;

public:
    static std::vector<City> ReadJSONFile(const std::string& file_path);
};



#endif // UTILS_HPP