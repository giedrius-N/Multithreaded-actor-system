#include "Printer.hpp"

printer_actor_state::printer_actor_state(printer_actor::pointer selfptr) : self(selfptr) {}

printer_actor::behavior_type printer_actor_state::make_behavior()
{
    return {
        [this](caf::unit_t)
        {
            self->println("I am printer actor");
        }
    };
}