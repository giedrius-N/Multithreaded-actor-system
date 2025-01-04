#ifndef RESULT_ACCUMULATOR_HPP
#define RESULT_ACCUMULATOR_HPP

#include "caf/typed_actor.hpp"
#include "caf/actor_system.hpp"
#include "caf/typed_event_based_actor.hpp"
#include "CityTypes.hpp"
#include "AtomConfig.hpp"
#include "Printer.hpp"
#include <unordered_set>
#include <chrono>

struct results_accumulator_actor_trait
{
    using signatures = caf::type_list<
        caf::result<void>(send_city, City, std::string),
        caf::result<void>(done_processing)
    >;
};
using results_accumulator_actor = caf::typed_actor<results_accumulator_actor_trait>;

struct results_accumulator_actor_state
{
    using clock = std::chrono::high_resolution_clock;
    using time_point = std::chrono::time_point<clock>;

    results_accumulator_actor::pointer self;
    printer_actor printer;

    std::vector<City> cities;
    int num_workers;
    int completed_workers = 0;
    int cities_from_python = 0;
    int cities_from_cpp = 0;

    time_point start_time;
    time_point end_time;

    std::unordered_set<std::string> city_ids;
    std::unordered_map<std::string, std::pair<bool, bool>> city_sources;

    explicit results_accumulator_actor_state(results_accumulator_actor::pointer selfptr, int workersCnt, printer_actor printer);

    results_accumulator_actor::behavior_type make_behavior();
};

#endif // RESULT_ACCUMULATOR_HPP
