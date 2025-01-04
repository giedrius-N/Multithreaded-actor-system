#include "Sender.hpp"
#include "caf/actor_ostream.hpp"

sender_actor_state::sender_actor_state(sender_actor::pointer selfptr) : self(selfptr) {}

void sender_actor_state::start_socket_server() 
{
    running_ = true;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        self->println("WSAStartup failed with error: {}", WSAGetLastError());
        return;
    }

    SOCKET server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sock == INVALID_SOCKET) 
    {
        self->println("socket() failed: {}", WSAGetLastError());
        WSACleanup();
        return;
    }

    sockaddr_in server_addr;
    ZeroMemory(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3490);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) 
    {
        self->println("bind() failed: {}", WSAGetLastError());
        closesocket(server_sock);
        WSACleanup();
        return;
    }

    if (listen(server_sock, SOMAXCONN) == SOCKET_ERROR) 
    {
        self->println("listen() failed: {}", WSAGetLastError());
        closesocket(server_sock);
        WSACleanup();
        return;
    }

    self->println("Socket server listening on port 3490...");

    sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    SOCKET client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_len);

    if (client_sock == INVALID_SOCKET) 
    {
        self->println("accept() failed: {}", WSAGetLastError());
        closesocket(server_sock);
        WSACleanup();
        return;
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    std::string connectionStr = "Got connection from " + std::string(client_ip);
    self->println(connectionStr);

    const char* greeting = "Hello from C++";
    int sent = send(client_sock, greeting, static_cast<int>(strlen(greeting)), 0);

    if (sent == SOCKET_ERROR) 
    {
        self->println("Failed to send greeting: {}", WSAGetLastError());
        closesocket(client_sock);
        closesocket(server_sock);
        WSACleanup();
        return;
    }

    self->println("Sent greeting to client.");

    char buffer[1024];
    int received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);

    if (received > 0) 
    {
        buffer[received] = '\0';
        std::string ackMessage = "Received ACK: " + std::string(buffer);
        self->println(ackMessage);

        if (std::string(buffer) == "ACK") 
        {
            self->println("ACK received. Shutting down.");
        } 
        else 
        {
            self->println("Unexpected message: {}", buffer);
        }
    } 
    else 
    {
        self->println("Failed to receive ACK: {}", WSAGetLastError());
    }

    closesocket(client_sock);
    closesocket(server_sock);
    WSACleanup();
    self->println("Server socket shut down.");
}


sender_actor::behavior_type sender_actor_state::make_behavior() 
{
    return 
    {
        [this](send_cities, std::string cities) 
        {
            self->println("Received cities, starting socket server...");
            start_socket_server();
            
        },
        [this](caf::unit_t) 
        {
            self->println("Sender actor triggered with unit_t");
        }
    };
}
