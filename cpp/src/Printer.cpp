/*
* Printer.cpp
* Implementation of the printer actor that prints the results after processing the data
*/
#include "Printer.hpp"
#include "Utils.hpp"

printer_actor_state::printer_actor_state(printer_actor::pointer selfptr) : self(selfptr) {}

void printer_actor_state::print_cities(const std::string& jsonStr)
{
    Utils::PrintCities(jsonStr, "Results after processing");

    self->quit();
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