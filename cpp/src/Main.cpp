#include "caf/all.hpp"
#include "MainActor.hpp"
#include "ResultAccumulator.hpp"
#include "Getter.hpp"

using namespace caf;
using namespace std::literals;

void caf_main(actor_system& sys)
{
    int workersCnt = 3;

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

    self->mail(send_json_path_v, "..\\..\\..\\data\\dataset_1.json").send(main_actor);
    self->mail(start_processing_v).send(main_actor);

    self->mail(get_cities_from_python_v).send(getter);
}

CAF_MAIN(id_block::system)
