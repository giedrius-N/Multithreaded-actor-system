/*
* Getter.hpp
* Declaration of the getter actor that receives data from Python
*/
#ifndef GETTER_HPP
#define GETTER_HPP

#include "caf/typed_actor.hpp"
#include "caf/event_based_actor.hpp"
#include "CityTypes.hpp"
#include "AtomConfig.hpp"
#include "ResultAccumulator.hpp"
#include <atomic>

struct getter_actor_trait
{
    using signatures = type_list<
        result<void>(get_cities_from_python)
    >;
};
using getter_actor = typed_actor<getter_actor_trait>;

struct getter_actor_state
{
    getter_actor::pointer self;
    results_accumulator_actor results_accumulator; 

    explicit getter_actor_state(getter_actor::pointer selfptr, results_accumulator_actor accumulator);

    std::atomic<bool> running_{false};
    void start_socket_server();
    void send_items_to_results(const std::string &jsonStr);

    getter_actor::behavior_type make_behavior();
};


#endif