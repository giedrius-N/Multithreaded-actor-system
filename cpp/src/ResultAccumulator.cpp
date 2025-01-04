#include "ResultAccumulator.hpp"
#include "caf/actor_ostream.hpp"
#include "Utils.hpp"

results_accumulator_actor_state::results_accumulator_actor_state(results_accumulator_actor::pointer selfptr, int workersCnt, printer_actor printer)
    : self(selfptr), num_workers(workersCnt), printer(std::move(printer))  {}

results_accumulator_actor::behavior_type results_accumulator_actor_state::make_behavior()
{
    return 
    {
        [this](caf::unit_t)
        {
            self->println("I am results_accumulator actor {}", cities.size());
        },
        [this](send_city, City city)
        {
            if (city_ids.insert(city.id).second)
            {
                cities.push_back(city);
            }
        },
        [this](done_processing)
        {
            completed_workers++;
            self->println("Worker done, total completed: {}", completed_workers);

            if (completed_workers == num_workers + 1) 
            {
                self->println("All workers completed. Total cities: {}", cities.size());
                
                std::string serializedCities = Utils::SerializeCities(cities);

                self->mail(send_printer_v, serializedCities).send(printer);
            }
        }
    };
}
