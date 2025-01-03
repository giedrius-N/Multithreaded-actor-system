#include "caf/actor_ostream.hpp"
#include "caf/actor_system.hpp"
#include "caf/caf_main.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/actor_from_state.hpp"
#include "caf/typed_actor.hpp"
#include "caf/all.hpp"

#include "CityTypes.hpp"
#include "Utils.hpp"
#include "Worker.hpp"
#include "ResultAccumulator.hpp"

#include "AtomConfig.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <cstring> 

using namespace caf;
using namespace std::literals;

struct sender_actor_trait
{
    using signatures = type_list<result<void>(std::vector<City>), result<void>(caf::unit_t)>;
};
using sender_actor = typed_actor<sender_actor_trait>;

struct main_actor_trait
{
    using signatures = type_list<result<void>(std::string), result<void>(caf::unit_t)>;
};
using main_actor = typed_actor<main_actor_trait>;

struct sender_actor_state
{
    sender_actor::pointer self;

    explicit sender_actor_state(sender_actor::pointer selfptr) : self(selfptr) {}

    void start_socket_server()
    {
        WSADATA wsaData;

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            self->println("WSAStartup failed with error: {}", WSAGetLastError());
            return;
        }

        SOCKET sockfd, new_fd;
        struct sockaddr_in server_addr, client_addr;
        int sin_size;
        char client_ip[INET_ADDRSTRLEN];
        int opt = 1;

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        {
            self->println("Socket creation failed with error: {}", WSAGetLastError());
            WSACleanup();
            return;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == SOCKET_ERROR)
        {
            self->println("setsockopt failed with error: {}", WSAGetLastError());
            closesocket(sockfd);
            WSACleanup();
            return;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(3490);
        server_addr.sin_addr.s_addr = INADDR_ANY;
        memset(&(server_addr.sin_zero), '\0', 8);

        if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
        {
            self->println("Bind failed with error: {}", WSAGetLastError());
            closesocket(sockfd);
            WSACleanup();
            return;
        }

        if (listen(sockfd, 10) == SOCKET_ERROR)
        {
            self->println("Listen failed with error: {}", WSAGetLastError());
            closesocket(sockfd);
            WSACleanup();
            return;
        }

        self->println("Socket server started, waiting for connections...");

        bool first_connection = true; // Track the first connection

        while (true)
        {
            sin_size = sizeof(client_addr);
            new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
            if (new_fd == INVALID_SOCKET)
            {
                self->println("Accept failed with error: {}", WSAGetLastError());
                continue;
            }

            inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, sizeof(client_ip));
            self->println("Got connection from {}", client_ip);

            const char *message = "HELLO FROM C++\n";
            if (send(new_fd, message, strlen(message), 0) == SOCKET_ERROR)
            {
                self->println("Send failed with error: {}", WSAGetLastError());
            }

            closesocket(new_fd);

            if (first_connection)
            {
                // Send a message to the actor to signal the first connection
                self->send(self, caf::unit_t{});
                first_connection = false; // Ensure this happens only for the first connection
            }
        }

        closesocket(sockfd);
        WSACleanup();
    }


    sender_actor::behavior_type make_behavior()
    {
        return {
            [this](std::vector<City> cities)
            {
                self->println("Received cities, starting socket server...");
                //std::thread(&sender_actor_state::start_socket_server, this).detach();
            },
            [this](caf::unit_t)
            {
                self->println("Sender actor triggered with unit_t");
            }
        };
    }
};

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
        return {
            [this](const std::string& file_path)
            {
                std::string jsonStr = Utils::ReadJSONFile(file_path);
                cities = Utils::GetCities(jsonStr);
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
            [this](caf::unit_t)
            {
                int sum = 0;
                if (!cities.empty())
                {
                    self->println("Distributing data to sender and workers...");
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
                }
                else
                {
                    self->println("No data to process.");
                }
            }
        };
    }
};

struct getter_actor_trait
{
    using signatures = type_list<result<void>(caf::unit_t)>;
};
using getter_actor = typed_actor<getter_actor_trait>;

struct printer_actor_trait
{
    using signatures = type_list<result<void>(caf::unit_t)>;
};
using printer_actor = typed_actor<printer_actor_trait>;

struct getter_actor_state
{
    getter_actor::pointer self;

    explicit getter_actor_state(getter_actor::pointer selfptr) : self(selfptr) {}

    getter_actor::behavior_type make_behavior()
    {
        return {
            [this](caf::unit_t)
            {
                self->println("I am getter actor");
            }
        };
    }
};

struct printer_actor_state
{
    printer_actor::pointer self;

    explicit printer_actor_state(printer_actor::pointer selfptr) : self(selfptr) {}

    printer_actor::behavior_type make_behavior()
    {
        return {
            [this](caf::unit_t)
            {
                self->println("I am printer actor");
            }
        };
    }
};

void caf_main(actor_system& sys)
{
    int workersCnt = 3;

    auto results_accumulator = sys.spawn(actor_from_state<results_accumulator_actor_state>, workersCnt);

    auto sender = sys.spawn(actor_from_state<sender_actor_state>);

    std::vector<worker_actor> workers;
    workers.reserve(workersCnt);
    
    for (int i = 0; i < workersCnt; i++)
    {
        auto worker = sys.spawn(actor_from_state<worker_actor_state>, results_accumulator);
        workers.push_back(worker);
    }

    auto main_actor_hdl = sys.spawn(actor_from_state<main_actor_state>, sender, workers);

    
    auto getter = sys.spawn(actor_from_state<getter_actor_state>);
    auto printer = sys.spawn(actor_from_state<printer_actor_state>);

    scoped_actor self{sys};
    self->send(main_actor_hdl, "..\\..\\..\\data\\dataset_1.json");
    self->send(main_actor_hdl, caf::unit_t{});

    self->send(results_accumulator, caf::unit_t{});

    self->send(getter, caf::unit_t{});

    self->send(printer, caf::unit_t{});
}

CAF_MAIN(id_block::system)
