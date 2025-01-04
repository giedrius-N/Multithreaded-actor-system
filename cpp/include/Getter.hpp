#ifndef GETTER_HPP
#define GETTER_HPP

#include "caf/typed_actor.hpp"
#include "caf/event_based_actor.hpp"
#include "CityTypes.hpp"
#include "AtomConfig.hpp"
#include <atomic>

struct getter_actor_trait
{
    using signatures = type_list<result<void>(caf::unit_t)>;
};
using getter_actor = typed_actor<getter_actor_trait>;

struct getter_actor_state
{
    getter_actor::pointer self;

    explicit getter_actor_state(getter_actor::pointer selfptr);

    std::atomic<bool> running_{false};
    void start_socket_server();

    getter_actor::behavior_type make_behavior();
};


#endif