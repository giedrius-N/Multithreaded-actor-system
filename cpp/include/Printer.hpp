#ifndef PRINTER_HPP
#define PRINTER_HPP

#include "caf/typed_actor.hpp"
#include "caf/actor_system.hpp"
#include "caf/typed_event_based_actor.hpp"
#include "CityTypes.hpp"
#include "AtomConfig.hpp"


struct printer_actor_trait
{
    using signatures = type_list<result<void>(caf::unit_t)>;
};
using printer_actor = typed_actor<printer_actor_trait>;

struct printer_actor_state
{
    printer_actor::pointer self;

    explicit printer_actor_state(printer_actor::pointer selfptr);

    printer_actor::behavior_type make_behavior();
};

#endif // PRINTER_HPP