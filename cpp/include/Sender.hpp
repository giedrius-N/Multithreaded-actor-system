/*
* Sender.hpp
* Declaration of the sender actor that sends the processed data to Python server
*/
#ifndef SENDER_HPP
#define SENDER_HPP

#include "caf/typed_actor.hpp"
#include "caf/event_based_actor.hpp"
#include <atomic>

#include "CityTypes.hpp"
#include "AtomConfig.hpp"

struct sender_actor_trait 
{
    using signatures = caf::type_list<
        caf::result<void>(send_cities, std::string cities)
    >;
};
using sender_actor = caf::typed_actor<sender_actor_trait>;

struct sender_actor_state 
{
    sender_actor::pointer self;

    explicit sender_actor_state(sender_actor::pointer selfptr);

    std::atomic<bool> running_{false};
    void start_socket_server(const std::string& message);

    sender_actor::behavior_type make_behavior();
};

#endif