/*
* SocketUtils.cpp
* Implementation of the socket utility functions for creating and managing sockets
*/
#include "SocketUtils.hpp"
#include <iostream>

bool SocketUtils::InitializeWinsock() 
{
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

SOCKET SocketUtils::CreateServerSocket(int port) 
{
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) 
    {
        return INVALID_SOCKET;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR ||
        listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) 
    {
        closesocket(serverSocket);
        return INVALID_SOCKET;
    }

    return serverSocket;
}

SOCKET SocketUtils::AcceptClient(SOCKET serverSocket, std::string& clientIp) 
{
    sockaddr_in clientAddr;
    int clientLen = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);

    if (clientSocket != INVALID_SOCKET) 
    {
        char ipBuffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), ipBuffer, INET_ADDRSTRLEN);
        clientIp = std::string(ipBuffer);
    }

    return clientSocket;
}

bool SocketUtils::SendMessage(SOCKET clientSocket, const std::string& message) 
{
    uint32_t messageSize = static_cast<uint32_t>(message.size());
    messageSize = htonl(messageSize);
    if (send(clientSocket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize), 0) == SOCKET_ERROR) 
    {
        return false;
    }

    size_t bytesSent = 0;
    while (bytesSent < message.size()) 
    {
        int sent = send(clientSocket, message.data() + bytesSent, static_cast<int>(message.size() - bytesSent), 0);
        if (sent == SOCKET_ERROR) 
        {
            return false;
        }
        bytesSent += sent;
    }

    return true;
}

std::string SocketUtils::ReceiveMessage(SOCKET clientSocket) 
{
    uint32_t messageSize = 0;
    if (recv(clientSocket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize), 0) <= 0) 
    {
        return {};
    }
    messageSize = ntohl(messageSize);

    std::string message;
    message.resize(messageSize);

    size_t bytesReceived = 0;
    while (bytesReceived < messageSize) 
    {
        int received = recv(clientSocket, &message[bytesReceived], static_cast<int>(messageSize - bytesReceived), 0);
        if (received <= 0) 
        {
            return {};
        }
        bytesReceived += received;
    }

    return message;
}



void SocketUtils::CleanupSocket(SOCKET socket) 
{
    closesocket(socket);
}

void SocketUtils::CleanupWinsock() 
{
    WSACleanup();
}
