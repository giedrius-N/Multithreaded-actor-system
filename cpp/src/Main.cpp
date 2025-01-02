#include "caf/actor_ostream.hpp"
#include "caf/actor_system.hpp"
#include "caf/caf_main.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/json_reader.hpp"
#include "caf/actor_from_state.hpp"
#include "caf/typed_actor.hpp"
#include "caf/all.hpp"

#include "CityTypes.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace caf;
using namespace std::literals;

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
        std::cout << city.id << " - " << city.name << ": " << city.sunnyDays
                  << " sunny days, " << city.averageTemp << " average temp" << std::endl;
    }
    return cities;
}
struct sender_actor_trait 
{
    using signatures = type_list<result<void>(std::vector<City>)>;
};
using sender_actor = typed_actor<sender_actor_trait>;

// Worker actor trait
struct worker_actor_trait 
{
    using signatures = type_list<result<void>(City)>;
};
using worker_actor = typed_actor<worker_actor_trait>;

// Sender actor state
struct sender_actor_state 
{
    sender_actor::pointer self;

    explicit sender_actor_state(sender_actor::pointer selfptr) : self(selfptr) {}

    sender_actor::behavior_type make_behavior() 
    {
        return 
        {
            [this](std::vector<City> cities) 
            {
                self->println("Sending data to Python:");
                for (const auto& city : cities) 
                {
                    self->println("- Sending city: {}", city.name);
                }
            }
        };
    }
};

// Worker actor state
struct worker_actor_state 
{
    worker_actor::pointer self;

    explicit worker_actor_state(worker_actor::pointer selfptr) : self(selfptr) {}

    worker_actor::behavior_type make_behavior() 
    {
        return 
        {
            [this](City city) 
            {
                self->println("Processing city in worker: {}", city.name);
            }
        };
    }
};

// Main actor trait
struct main_actor_trait 
{
    using signatures = type_list<
        result<void>(std::string),
        result<void>(caf::unit_t)>;
};
using main_actor = typed_actor<main_actor_trait>;

struct main_actor_state 
{
    main_actor::pointer self;
    sender_actor sender;
    std::vector<worker_actor> workers;
    std::vector<City> cities;

    main_actor_state(main_actor::pointer selfptr, sender_actor s, std::vector<worker_actor> w)
        : self(selfptr), sender(std::move(s)), workers(std::move(w)) {}

    main_actor::behavior_type make_behavior() 
    {
        return 
        {
            [this](const std::string& file_path) 
            {
                cities = read_json_file(file_path);
                if (!cities.empty()) 
                {
                    self->println("File loaded successfully: {}", file_path);
                } else 
                {
                    self->println("Failed to load file: {}", file_path);
                    self->quit();
                }
            },
            [this](caf::unit_t) 
            {
                if (!cities.empty()) 
                {
                    self->println("Distributing data to sender and workers...");

                    self->mail(cities).send(sender);

                    for (size_t i = 0; i < cities.size(); ++i) 
                    {
                        self->mail(cities[i]).send(workers[i % workers.size()]);
                    }
                } else 
                {
                    self->println("No data to process.");
                }
            }
        };
    }
};

void caf_main(actor_system& sys) 
{
    auto sender = sys.spawn(actor_from_state<sender_actor_state>);
    auto worker1 = sys.spawn(actor_from_state<worker_actor_state>);
    auto worker2 = sys.spawn(actor_from_state<worker_actor_state>);
    auto worker3 = sys.spawn(actor_from_state<worker_actor_state>);
    std::vector<worker_actor> workers = {worker1, worker2, worker3};

    auto main_actor_hdl = sys.spawn(actor_from_state<main_actor_state>, sender, workers);

    scoped_actor self{sys};
    std::string file_path = "..\\..\\..\\data\\dataset_1.json";

    self->send(main_actor_hdl, file_path);
    self->send(main_actor_hdl, caf::unit_t{});
}

CAF_MAIN(id_block::custom_types)
