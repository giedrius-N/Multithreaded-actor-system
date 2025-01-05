/*
* MainActor.hpp
* Declaration of the main actor that processes the data and sends it to the workers and sender
*/
#ifndef MAIN_ACTOR_HPP
#define MAIN_ACTOR_HPP

#include "caf/typed_actor.hpp"
#include "caf/actor_system.hpp"
#include "caf/typed_event_based_actor.hpp"
#include "CityTypes.hpp"
#include "AtomConfig.hpp"
#include "Sender.hpp"
#include "Worker.hpp"

struct main_actor_trait
{
    using signatures = type_list<
        result<void>(send_json_path, std::string), 
        result<void>(start_processing)
    >;
};
using main_actor = typed_actor<main_actor_trait>;

struct main_actor_state
{
    main_actor::pointer self;
    sender_actor sender;
    std::vector<worker_actor> workers;
    std::vector<City> cities;

    main_actor_state(main_actor::pointer selfptr, sender_actor s, std::vector<worker_actor> w);

    main_actor::behavior_type make_behavior();
};

#endif 