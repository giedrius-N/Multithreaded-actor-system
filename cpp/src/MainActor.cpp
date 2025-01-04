#include "MainActor.hpp"
#include "Utils.hpp"


main_actor_state::main_actor_state(main_actor::pointer selfptr, sender_actor s, std::vector<worker_actor> w)
        : self(selfptr), sender(std::move(s)), workers(std::move(w)) {}


main_actor::behavior_type main_actor_state::make_behavior()
{
    return 
    {
        [this](send_json_path, const std::string& file_path)
        {
            std::string jsonStr = Utils::ReadJSONFile(file_path);

            self->mail(send_cities_v, jsonStr).send(sender);

            cities = Utils::ParseCities(jsonStr);
            if (!cities.empty())
            {
                self->println("File loaded successfully: {}", file_path);
            }
            else
            {
                self->println("Failed to load file: {}", file_path);
                self->quit();
            }
        },
        [this](start_processing)
        {
            int sum = 0;
            if (!cities.empty())
            {
                int num_workers = workers.size();
                int total_items = cities.size();
                
                int base_size = total_items / num_workers;
                int extra_items = total_items % num_workers;
                
                auto current = cities.begin();
                
                for (int i = 0; i < num_workers; ++i) 
                {
                    int worker_batch_size = base_size + (i < extra_items ? 1 : 0);
                    
                    std::vector<City> batch(current, current + worker_batch_size);
                    if (!batch.empty()) 
                    {
                        self->mail(send_cities_v, batch).send(workers[i]);
                    }
                    
                    current += worker_batch_size;
                }

                self->quit();
            }
            else
            {
                self->println("No data to process.");
            }
        }
    };
}
