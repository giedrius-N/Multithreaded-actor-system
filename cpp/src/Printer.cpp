#include "Printer.hpp"
#include <fstream>
#include <iomanip>
#include "Utils.hpp"

printer_actor_state::printer_actor_state(printer_actor::pointer selfptr) : self(selfptr) {}

void printer_actor_state::print_cities(const std::string& jsonStr)
{
    std::vector<City> cities = Utils::ParseCities(jsonStr);
    if (cities.empty()) 
    {
        self->println("No cities to print.");
        return;
    }
    std::ofstream file("..\\..\\..\\results\\results.txt");
    if (!file.is_open()) 
    {
        self->println("Failed to open results file.");
        return;
    }

    file << std::left
         << std::setw(10) << "ID"
         << std::setw(20) << "Name"
         << std::setw(15) << "Sunny Days"
         << std::setw(15) << "Avg Temp"
         << "\n";

    file << std::string(60, '-') << "\n";

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

 

printer_actor::behavior_type printer_actor_state::make_behavior()
{
    return {
        [this](send_printer, std::string message)
        {
            print_cities(message);
        }
    };
}