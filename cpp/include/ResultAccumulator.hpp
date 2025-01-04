#ifndef RESULT_ACCUMULATOR_HPP
#define RESULT_ACCUMULATOR_HPP

#include "caf/typed_actor.hpp"
#include "caf/actor_system.hpp"
#include "caf/typed_event_based_actor.hpp"
#include "CityTypes.hpp"
#include "AtomConfig.hpp"
#include <unordered_set>

struct results_accumulator_actor_trait
{
    using signatures = caf::type_list<
        caf::result<void>(caf::unit_t), 
        caf::result<void>(send_city, City),
        caf::result<void>(std::string)
    >;
};
using results_accumulator_actor = caf::typed_actor<results_accumulator_actor_trait>;

struct results_accumulator_actor_state
{
    results_accumulator_actor::pointer self;

    std::vector<City> cities;
    int num_workers;
    int completed_workers = 0;

    std::unordered_set<std::string> city_ids;

    explicit results_accumulator_actor_state(results_accumulator_actor::pointer selfptr, int workersCnt);

    results_accumulator_actor::behavior_type make_behavior();
};

#endif // RESULT_ACCUMULATOR_HPP
