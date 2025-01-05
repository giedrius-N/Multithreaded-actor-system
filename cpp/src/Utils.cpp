/*
* Utils.cpp
* Implementation of the utility functions for reading and writing JSON files
*/
#include "Utils.hpp"
#include "caf/json_reader.hpp"
#include "caf/json_writer.hpp"
#include <fstream>
#include <iomanip>

std::string Utils::ReadJSONFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fileContent = buffer.str();

    file.close();

    PrintCities(fileContent, "Initial data");

    return fileContent;
}

void Utils::PrintCities(const std::string& jsonStr, const std::string& header)
{
    static bool first_use = true;
    std::ios_base::openmode mode = first_use ? std::ios::trunc : std::ios::app;
    first_use = false;

    std::ofstream file("..\\..\\..\\results\\results.txt", mode);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open results file." << std::endl;
        return;
    }

    std::vector<City> cities = Utils::ParseCities(jsonStr);
    if (cities.empty()) 
    {
        file << std::string(60, '-') << "\n";
        file << "Results are empty." << "\n";
        file << std::string(60, '-') << "\n";
        file.close();
        return;
    }

    file << std::string(60, '-') << "\n";
    file << header << ". Total amount of items: " << cities.size() <<  "\n";
    file << std::string(60, '-') << "\n";
    if (mode == std::ios::trunc) 
    {
        file << std::left
             << std::setw(10) << "ID"
             << std::setw(20) << "Name"
             << std::setw(15) << "Sunny Days"
             << std::setw(15) << "Avg Temp"
             << "\n";
        file << std::string(60, '-') << "\n";
    }

    for (const auto& city : cities) 
    {
        file << std::left
             << std::setw(10) << city.id
             << std::setw(20) << city.name
             << std::setw(15) << city.sunnyDays
             << std::setw(15) << city.averageTemp
             << "\n";
    }

    file.close();
}


std::vector<City> Utils::ParseCities(const std::string& jsonString)
{
    caf::json_reader reader;
    if (!reader.load(jsonString))
    {
        std::cerr << "Could not parse JSON" << std::endl;
        std::cerr << "Error: " << to_string(reader.get_error()) << std::endl;
        return {};
    }

    std::vector<City> cities;
    if (!reader.apply(cities))
    {
        std::cerr << "Could not convert JSON to cities" << std::endl;
        return {};
    }

    return cities;
}

std::string Utils::SerializeCities(const std::vector<City>& cities)
{
    caf::json_writer writer;

    if (!writer.apply(cities))
    {
        std::cerr << "Could not serialize cities to JSON" << std::endl;
        return "";
    }

    return std::string(writer.str());
}