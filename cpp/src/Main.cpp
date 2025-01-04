// Processor: 11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz (8 CPUs), ~3.1GHz
// - With one worker calculations and filtering (C++ side) completed in 11.5282 seconds
// - With four workers the entire program completed in 12.6017 seconds

#include "caf/all.hpp"
#include "MainActor.hpp"
#include "ResultAccumulator.hpp"
#include "Getter.hpp"
#include <chrono>

using namespace caf;
using namespace std::literals;

void caf_main(actor_system& sys)
{
    auto start = std::chrono::high_resolution_clock::now();

    int workersCnt = 4;

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

    self->mail(send_json_path_v, "..\\..\\..\\data\\dataset_2.json").send(main_actor);
    self->mail(start_processing_v).send(main_actor);

    self->mail(get_cities_from_python_v).send(getter);

    self->await_all_other_actors_done();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    self->println("Total program time: {}s", elapsed.count());
}

CAF_MAIN(id_block::system)
