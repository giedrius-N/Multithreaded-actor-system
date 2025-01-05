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
    static std::vector<City> ParseCities(const std::string& jsonString);
    static std::string SerializeCities(const std::vector<City>& cities);
    static void PrintCities(const std::string& jsonStr, const std::string& header);
};



#endif // UTILS_HPP