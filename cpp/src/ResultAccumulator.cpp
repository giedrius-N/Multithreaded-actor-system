/*
* ResultAccumulator.cpp
* Result accumulator actor implementation that accumulates the results from the worker actors and getter actor and sends the data to the printer actor for printing
*/
#include "ResultAccumulator.hpp"
#include "caf/actor_ostream.hpp"
#include "Utils.hpp"

results_accumulator_actor_state::results_accumulator_actor_state(results_accumulator_actor::pointer selfptr, int workersCnt, printer_actor printer)
    : self(selfptr), num_workers(workersCnt), printer(std::move(printer))  
    {
        start_time = clock::now();
    }

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
            }
        },
        [this](done_processing)
        {
            completed_workers++;

            // All workers + 1 for the getter - items from python
            // This is triggered when all C++ workers have finished processing
            // and the getter has finished processing the items from Python
            if (completed_workers == num_workers + 1) 
            {
                self->println("All workers completed. Total cities that matches both filters: {}", cities.size());
                self->println("Cities matching the filter from Python: {}", cities_from_python);
                self->println("Cities matching the filter from C++: {}", cities_from_cpp);

                std::string serializedCities = Utils::SerializeCities(cities);

                self->mail(send_printer_v, serializedCities).send(printer);

                self->quit();
            }
            else if (completed_workers == num_workers)
            {
                end_time = clock::now();
                std::chrono::duration<double> elapsed = end_time - start_time;
                self->println("All workers completed. Processing time: {} seconds", elapsed.count());
            }
        }
    };
}
