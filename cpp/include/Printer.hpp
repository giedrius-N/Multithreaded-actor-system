#ifndef PRINTER_HPP
#define PRINTER_HPP

#include "caf/typed_actor.hpp"
#include "caf/actor_system.hpp"
#include "caf/typed_event_based_actor.hpp"
#include "CityTypes.hpp"
#include "AtomConfig.hpp"


struct printer_actor_trait
{
    using signatures = type_list<
        result<void>(send_printer, std::string)
    >;
};
using printer_actor = typed_actor<printer_actor_trait>;

struct printer_actor_state
{
    printer_actor::pointer self;
    // Method to print cities from json string nicely formated
    void print_cities(const std::string& jsonStr);

    explicit printer_actor_state(printer_actor::pointer selfptr);

    printer_actor::behavior_type make_behavior();
};

#endif // PRINTER_HPP