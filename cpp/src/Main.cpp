#include "caf/actor_ostream.hpp"
#include "caf/actor_system.hpp"
#include "caf/caf_main.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/json.hpp"

#include <fstream>
#include <iostream>
#include <vector>

using namespace caf;
using namespace std::literals;

struct City
{
  std::string name;
  int sunnyDays;
  double averageTemp;
};

template <typename Inspector>
bool inspect(Inspector& f, City& x)
{
  return f.object(x).fields(
    f.field("name", x.name),
    f.field("sunnyDays", x.sunnyDays),
    f.field("averageTemp", x.averageTemp)
  );
}

std::vector<City> read_json_file(const std::string& file_path)
{
  std::ifstream file(file_path);
  if (!file.is_open()) 
  {
    std::cerr << "Could not open file: " << file_path << std::endl;
    return {};
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string file_content = buffer.str();

  caf::json_reader reader;
  if (!reader.load(file_content))
  {
    std::cerr << "Could not parse file: " << file_path << std::endl;
    std::cerr << "Error: " << to_string(reader.get_error()) << std::endl;
    return {};
  }

  std::vector<City> cities;
  if (!reader.apply(cities))
  {
    std::cerr << "Could not convert JSON to cities" << std::endl;
    return{};
  }

  std::cout << "Loaded cities from file: " << file_path << std::endl;
  for (const auto& city : cities) 
  {
    std::cout << "- " << city.name << ": " << city.sunnyDays
              << " sunny days, " << city.averageTemp << " average temp" << std::endl;
  }

  return cities;
}

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

  std::vector<City> cities = read_json_file("C:\\Users\\eilug\\OneDrive\\Desktop\\LygiagAlternatyva\\Multithreaded-actor-system\\data\\dataset_1.json");
}

CAF_MAIN()