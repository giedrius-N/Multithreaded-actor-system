/*
* SocketUtils.hpp
* Utility class for socket operations
*/
#ifndef SOCKET_UTILS_HPP
#define SOCKET_UTILS_HPP

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

class SocketUtils 
{
private:
    SocketUtils() = delete;
    SocketUtils(const SocketUtils&) = delete;
    SocketUtils& operator=(const SocketUtils&) = delete;

public:
    static bool InitializeWinsock();

    static SOCKET CreateServerSocket(int port);

    static SOCKET AcceptClient(SOCKET serverSocket, std::string& clientIp);

    static bool SendMessage(SOCKET clientSocket, const std::string& message);

    static std::string ReceiveMessage(SOCKET clientSocket);

    static void CleanupSocket(SOCKET socket);

    static void CleanupWinsock();
};

#endif // SOCKET_UTILS_HPP
