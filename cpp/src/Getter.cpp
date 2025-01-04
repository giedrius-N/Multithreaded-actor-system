#include "Getter.hpp"
#include "SocketUtils.hpp"
#include "Utils.hpp"

getter_actor_state::getter_actor_state(getter_actor::pointer selfptr, results_accumulator_actor accumulator)
     : self(selfptr), results_accumulator(std::move(accumulator)) {}

void getter_actor_state::start_socket_server()
{
    running_ = true;

    if (!SocketUtils::InitializeWinsock())
    {
        self->println("WSAStartup failed.");
        self->quit();
        return;
    }

    SOCKET serverSocket = SocketUtils::CreateServerSocket(3491);
    if (serverSocket == INVALID_SOCKET)
    {
        self->println("Failed to create server socket.");
        SocketUtils::CleanupWinsock();
        self->quit();
        return;
    }

    self->println("Getter socket server listening on port 3491...");

    while (running_)
    {
        std::string clientIp;
        SOCKET clientSocket = SocketUtils::AcceptClient(serverSocket, clientIp);

        if (clientSocket == INVALID_SOCKET)
        {
            self->println("Failed to accept client connection.");
            continue;
        }

        self->println("Got connection from {}", clientIp);

        std::string message = SocketUtils::ReceiveMessage(clientSocket);
        if (!message.empty())
        {
            send_items_to_results(message);
            running_ = false;
        }
        else
        {
            self->println("Failed to receive message from client.");
        }

        SocketUtils::CleanupSocket(clientSocket);
    }

    SocketUtils::CleanupSocket(serverSocket);
    SocketUtils::CleanupWinsock();

    self->println("Getter socket server shut down.");
}

void getter_actor_state::send_items_to_results(const std::string &jsonStr)
{
    if (jsonStr == "[]")
    {
        self->println("Received empty JSON from Python.");
        self->mail(done_processing_v).send(results_accumulator);
        return;
    }

    std::vector<City> cities = Utils::ParseCities(jsonStr);
    if (cities.empty())
    {
        self->println("Failed to parse cities from JSON.");
        return;
    }

    for (const auto &city : cities)
    {
        self->mail(send_city_v, city, "python").send(results_accumulator);
    }

    self->mail(done_processing_v).send(results_accumulator);
}

getter_actor::behavior_type getter_actor_state::make_behavior()
{
    return 
    {
        [this](get_cities_from_python)
        {
            start_socket_server();
        }
    };
}