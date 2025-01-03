#include "Utils.hpp"
#include "caf/json_reader.hpp"

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

    return fileContent;
}

std::vector<City> Utils::GetCities(const std::string& jsonString)
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