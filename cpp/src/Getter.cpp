#include "Getter.hpp"
#include "SocketUtils.hpp"

getter_actor_state::getter_actor_state(getter_actor::pointer selfptr) : self(selfptr) {}

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
            self->println("Received message: {}", message);
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
    self->quit();
}

getter_actor::behavior_type getter_actor_state::make_behavior()
{
    return 
    {
        [this](caf::unit_t)
        {
            start_socket_server();
        }
    };
}