#include "Worker.hpp"
#include "caf/actor_ostream.hpp"
#include "AtomConfig.hpp"

worker_actor_state::worker_actor_state(worker_actor::pointer selfptr, results_accumulator_actor accumulator)
    : self(selfptr), results_accumulator(std::move(accumulator)) {}

void worker_actor_state::process_cities(std::vector<City>& cities)
{
    for (const auto& city : cities)
    {
        int comfortIndex = calculate_comfort_index(city);

        if (comfortIndex > 323) // Check if average temp index is greater than 18 degrees
        {
            self->mail(send_city_v, city, "cpp").send(results_accumulator);
        }
    }

    self->mail(done_processing_v).send(results_accumulator);
}

int worker_actor_state::calculate_comfort_index(const City& city)
{
    double averageTemp = city.averageTemp;

    // Mock calculation for mocking long running operation
    for (int i = 0; i < 10000000; i++)
    {
        averageTemp += 0.1;
        averageTemp -= 0.1;
    }

    return averageTemp * averageTemp;
}

worker_actor::behavior_type worker_actor_state::make_behavior()
{
    return 
    {
        [this](City city)
        {
            std::vector cities = {city};
            process_cities(cities);
        },
        [this](caf::unit_t)
        {
            self->println("Worker actor triggered with unit_t");
        },
        [this](send_cities, std::vector<City> cities)
        {
            process_cities(cities);
        }
    };
}
