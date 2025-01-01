#include "caf/actor_ostream.hpp"
#include "caf/actor_system.hpp"
#include "caf/caf_main.hpp"
#include "caf/event_based_actor.hpp"

using namespace caf;
using namespace std::literals;

behavior mirror(event_based_actor* self) 
{
  return 
  {
    [self](const std::string& what) -> std::string 
    {
      self->println("{}", what);
      return std::string{what.rbegin(), what.rend()};
    },
  };
}

void hello_world(event_based_actor* self, const actor& buddy) 
{
  self->mail("Hello World!")
    .request(buddy, 10s)
    .then(
      [self](const std::string& what) 
      {
        self->println("{}", what);
      });
}

void caf_main(actor_system& sys) 
{
  auto mirror_actor = sys.spawn(mirror);
  sys.spawn(hello_world, mirror_actor);
}

CAF_MAIN()