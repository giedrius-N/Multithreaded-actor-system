#include "Utils.hpp"
#include "caf/json_reader.hpp"

std::vector<City> Utils::ReadJSONFile(const std::string& file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << file_path << std::endl;
        return {};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string file_content = buffer.str();

    caf::json_reader reader;
    if (!reader.load(file_content))
    {
        std::cerr << "Could not parse file: " << file_path << std::endl;
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