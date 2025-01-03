#ifndef WORKER_HPP
#define WORKER_HPP

#include "caf/typed_actor.hpp"
#include "caf/actor_system.hpp"
#include "CityTypes.hpp"

struct worker_actor_trait
{
    using signatures = caf::type_list<caf::result<void>(City), caf::result<void>(caf::unit_t), caf::result<void>(std::vector<City>)>;
};
using worker_actor = caf::typed_actor<worker_actor_trait>;

struct worker_actor_state
{
    worker_actor::pointer self;

    explicit worker_actor_state(worker_actor::pointer selfptr) : self(selfptr) {}

    worker_actor::behavior_type make_behavior();

    void process_cities(std::vector<City>& cities);
    int calculate_comfort_index(const City& city);
};

#endif // WORKER_HPP
