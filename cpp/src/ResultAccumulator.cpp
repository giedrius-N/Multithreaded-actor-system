#include "ResultAccumulator.hpp"
#include "caf/actor_ostream.hpp"
#include "Utils.hpp"

results_accumulator_actor_state::results_accumulator_actor_state(results_accumulator_actor::pointer selfptr, int workersCnt, printer_actor printer)
    : self(selfptr), num_workers(workersCnt), printer(std::move(printer))  {}

results_accumulator_actor::behavior_type results_accumulator_actor_state::make_behavior()
{
    return 
    {
        [this](send_city, City city, std::string source)
        {
            auto& sources = city_sources[city.id];
            
            if (source == "cpp") 
            {
                sources.first = true;
                cities_from_cpp++;
            } 
            else if (source == "python") 
            {
                sources.second = true;
                cities_from_python++;
            }

            if (sources.first && sources.second) 
            {
                cities.push_back(city);
                self->println("City: {} added to accumulator from both sources", city.name);
            }
        },
        [this](done_processing)
        {
            completed_workers++;
            self->println("Worker done, total completed: {}", completed_workers);

            if (completed_workers == num_workers + 1) 
            {
                self->println("All workers completed. Total cities that matches both filters: {}", cities.size());
                self->println("Cities matching the filter from Python: {}", cities_from_python);
                self->println("Cities matching the filter from C++: {}", cities_from_cpp);

                std::string serializedCities = Utils::SerializeCities(cities);

                self->mail(send_printer_v, serializedCities).send(printer);
            }
        }
    };
}
