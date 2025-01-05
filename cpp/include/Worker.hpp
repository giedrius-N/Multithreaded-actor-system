/*
* Worker.hpp
* Declaration of the worker actor that processes the data and sends the results to the results accumulator actor
*/
#ifndef WORKER_HPP
#define WORKER_HPP

#include "caf/typed_actor.hpp"
#include "caf/actor_system.hpp"
#include "CityTypes.hpp"
#include "ResultAccumulator.hpp"
#include "AtomConfig.hpp"

struct worker_actor_trait
{
    using signatures = caf::type_list<
        caf::result<void>(send_cities, std::vector<City>)
    >;
};
using worker_actor = caf::typed_actor<worker_actor_trait>;

struct worker_actor_state
{
    worker_actor::pointer self;
    results_accumulator_actor results_accumulator; 

    explicit worker_actor_state(worker_actor::pointer selfptr, results_accumulator_actor accumulator);

    worker_actor::behavior_type make_behavior();

    void process_cities(std::vector<City>& cities);
    int calculate_comfort_index(const City& city);
};

#endif // WORKER_HPP
