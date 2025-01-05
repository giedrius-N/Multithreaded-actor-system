/*
* Main.cpp
* C++ implementation of the main actor system using C++ Actor Framework open-source library.
* The main function is responsible for creating the actors and sending the initial messages to start the processing.
*
* Processor: 11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz (8 CPUs), ~3.1GHz
* - With one worker the program completed in 11.5282 seconds
* - With four workers the program completed in 4.40966 seconds
*/
#include "caf/all.hpp"
#include "MainActor.hpp"
#include "ResultAccumulator.hpp"
#include "Getter.hpp"
#include <chrono>

using namespace caf;
using namespace std::literals;

// Program configuration through command line arguments
struct config : public caf::actor_system_config 
{
    int workers = 4;
    std::string dataset = "dataset_1.json";

    config() 
    {
        opt_group{custom_options_, "global"}
        .add(workers,  "workers,w",  "Number of worker actors" )
        .add(dataset,  "dataset,d",  "Path to JSON dataset"   );
    }
};

void caf_main(actor_system& sys, const config& cfg)
{
    auto start = std::chrono::high_resolution_clock::now();

    int workersCnt = cfg.workers;
    std::string datasetPath = "..\\..\\..\\data\\" + cfg.dataset;

    auto printer = sys.spawn(actor_from_state<printer_actor_state>);
    auto results_accumulator = sys.spawn(actor_from_state<results_accumulator_actor_state>, workersCnt, printer);
    auto sender = sys.spawn(actor_from_state<sender_actor_state>);

    std::vector<worker_actor> workers;
    workers.reserve(workersCnt);
    
    for (int i = 0; i < workersCnt; i++)
    {
        auto worker = sys.spawn(actor_from_state<worker_actor_state>, results_accumulator);
        workers.push_back(worker);
    }

    auto main_actor = sys.spawn(actor_from_state<main_actor_state>, sender, workers);
    auto getter = sys.spawn(actor_from_state<getter_actor_state>, results_accumulator);
    
    scoped_actor self{sys};

    self->mail(send_json_path_v, datasetPath).send(main_actor);
    self->mail(start_processing_v).send(main_actor);

    self->mail(get_cities_from_python_v).send(getter);

    self->await_all_other_actors_done();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    self->println("Total program time: {}s", elapsed.count());
}

CAF_MAIN(id_block::system)
