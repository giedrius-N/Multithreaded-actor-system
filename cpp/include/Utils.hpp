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
    static std::string ReadJSONFile(const std::string& filePath);
    static std::vector<City> GetCities(const std::string& jsonString);
};



#endif // UTILS_HPP