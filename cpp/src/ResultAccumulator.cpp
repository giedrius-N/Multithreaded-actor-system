#include "ResultAccumulator.hpp"
#include "caf/actor_ostream.hpp"

#include <fstream>

results_accumulator_actor_state::results_accumulator_actor_state(results_accumulator_actor::pointer selfptr, int workersCnt)
    : self(selfptr), num_workers(workersCnt) {}

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
            //cities.push_back(city);
            //self->println("City: {} added to accumulator", city.name);

            if (city_ids.insert(city.id).second)
            {
                cities.push_back(city);
            }
        },
        [this](std::string done_message)
        {
            completed_workers++;
            self->println("Worker done, total completed: {}", completed_workers);

            if (completed_workers == num_workers) 
            {
                self->println("All workers completed. Total cities: {}", cities.size());

                // Write to file a number of cities
                std::ofstream file("..\\..\\..\\results\\cities.txt");
                file << cities.size();
                file.close();

            }
        }
    };
}
