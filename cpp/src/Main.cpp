#include "caf/actor_ostream.hpp"
#include "caf/actor_system.hpp"
#include "caf/caf_main.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/json_reader.hpp"
#include "caf/actor_from_state.hpp"
#include "caf/typed_actor.hpp"
#include "caf/all.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace caf;
using namespace std::literals;

struct City
{
    int id;
    std::string name;
    int sunnyDays;
    double averageTemp;
};

template <class Inspector>
bool inspect(Inspector& f, City& x)
{
    return f.object(x).fields(
        f.field("id", x.id),
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
        return {};
    }

    std::cout << "Loaded cities from file: " << file_path << std::endl;
    for (auto& city : cities)
    {
        std::cout << city.id << " " << "- " << city.name << ": " << city.sunnyDays
                  << " sunny days, " << city.averageTemp
                  << " average temp" << std::endl;
    }
    return cities;
}

struct main_actor_trait
{
    using signatures = type_list<
      
        result<void>(std::string),
      
        result<void>(caf::unit_t)
    >;
};

using main_actor_t = typed_actor<main_actor_trait>;

struct main_actor_state
{
  
    std::string file_content;

    explicit main_actor_state(main_actor_t::pointer selfptr)
        : self(selfptr)
    {
      
    }

    main_actor_t::behavior_type make_behavior()
    {
        return {
            [this](const std::string& file_path)
            {
                if (load_file(file_path))
                {
                    self->println("File loaded successfully: {}", file_path);
                }
                else
                {
                    self->println("Failed to load file: {}", file_path);
                    self->quit();
                }
            },
            [this](caf::unit_t)
            {
                if (!file_content.empty())
                {
                    self->println("Processing JSON data: {}", file_content);
                    process_data();
                }
                else
                {
                    self->println("No JSON data to process.");
                }
            }
        };
    }
  
    bool load_file(const std::string& file_path)
    {
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file: " << file_path << '\n';
            return false;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        file_content = buffer.str();
        caf::json_reader reader;
        if (!reader.load(file_content))
        {
            std::cerr << "Failed to parse JSON file: " << file_path << '\n';
            return false;
        }
        std::cout << "Successfully loaded JSON file: " << file_path << '\n';
        return true;
    }

    void process_data()
    {
      
        std::cout << "Processing file contents: " << file_content << "\n";
    }

    main_actor_t::pointer self;
};

void caf_main(actor_system& sys)
{
    auto main_actor_hdl = sys.spawn(actor_from_state<main_actor_state>);

    scoped_actor self{sys};
    std::string file_path = "..\\..\\..\\data\\dataset_1.json";

    self->send(main_actor_hdl, file_path);
    self->send(main_actor_hdl, caf::unit_t{});
}

CAF_MAIN()
