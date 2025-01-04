#include "Sender.hpp"
#include "caf/actor_ostream.hpp"
#include "SocketUtils.hpp"

sender_actor_state::sender_actor_state(sender_actor::pointer selfptr) : self(selfptr) {}

void sender_actor_state::start_socket_server(const std::string& message) 
{
    if (!SocketUtils::InitializeWinsock()) 
    {
        self->println("WSAStartup failed.");
        self->quit();
        return;
    }

    SOCKET serverSocket = SocketUtils::CreateServerSocket(3490);
    if (serverSocket == INVALID_SOCKET) 
    {
        self->println("Failed to create server socket.");
        SocketUtils::CleanupWinsock();
        self->quit();
        return;
    }

    self->println("Socket server listening on port 3490...");

    std::string clientIp;
    SOCKET clientSocket = SocketUtils::AcceptClient(serverSocket, clientIp);

    if (clientSocket == INVALID_SOCKET) 
    {
        self->println("Failed to accept client connection.");
    } 
    else 
    {
        self->println("Got connection from {}", clientIp);

        if (SocketUtils::SendMessage(clientSocket, message)) 
        {
            self->println("Sent data to client.");

            std::string ack = SocketUtils::ReceiveMessage(clientSocket);
            if (!ack.empty()) 
            {
                self->println("Received from client: {}", ack);

                if (ack == "ACK") 
                {
                    self->println("ACK received.");
                } 
                else 
                {
                    self->println("Unexpected message: {}", ack);
                }
            } 
            else 
            {
                self->println("Failed to receive ACK.");
            }
        } 
        else 
        {
            self->println("Failed to send greeting.");
        }

        SocketUtils::CleanupSocket(clientSocket);
    }

    SocketUtils::CleanupSocket(serverSocket);
    SocketUtils::CleanupWinsock();
    self->println("Sender socket server shut down.");
    self->quit();
}



sender_actor::behavior_type sender_actor_state::make_behavior() 
{
    return 
    {
        [this](send_cities, std::string cities) 
        {
            self->println("Received cities, starting socket server...");

            start_socket_server(cities);
            
        },
        [this](caf::unit_t) 
        {
            self->println("Sender actor triggered with unit_t");
        }
    };
}
