//
// Created by Luis Tadeo Sanchez on 3/6/23.
//

#include "Socket.h"

#include <iostream>
#include <unistd.h>
#include <netdb.h>

namespace Shlib
{
    Socket::Socket()
    {
        m_SocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_SocketFD < 0)
            std::cout << "Error creating socket" << std::endl;
    }

    Socket::Socket(int socketfd)
        : m_SocketFD(socketfd)
    { }

    bool Socket::Listen(int port, int maxConnections)
    {
        sockaddr_in service{};
        service.sin_family = AF_INET;
        service.sin_port = htons(port);
        service.sin_addr.s_addr = INADDR_ANY;

        if (bind(m_SocketFD, (struct sockaddr*) &service, sizeof(service)) < 0)
        {
            std::cout << "Error on binding" << std::endl;
            return false;
        }

        listen(m_SocketFD, 5);

        return true;
    }

    Socket Socket::Accept()
    {
        Socket client(-1);

        client.m_SocketFD = accept(m_SocketFD, NULL, NULL);
        if (!client.IsValid())
            std::cout << "Error on accept" << std::endl;
    }

    bool Socket::Connect(const char *address, int port)
    {
        sockaddr_in service{};
        hostent* server = gethostbyname(address);

        if (!server)
        {
            std::cout << "Error finding server" << std::endl;
            return false;
        }

        service.sin_family = AF_INET;
        service.sin_port = htons(port);
        bcopy((char *)server->h_addr,
              (char *)&service.sin_addr.s_addr,
              server->h_length);

        if (connect(m_SocketFD, (const sockaddr*)&service, sizeof(service)) < 0)
        {
            std::cout << "Error connecting to server" << std::endl;
            return false;
        }

        return true;
    }

    bool Socket::ReceiveFrom(Socket socket, char *buffer, int size)
    {
        int numBytes = read(socket.m_SocketFD, buffer, size);

        if (numBytes < 0)
        {
            std::cout << "Error reading from socket" << std::endl;
            return false;
        }

        return true;
    }

    bool Socket::SendTo(Socket socket, char *data, int size)
    {
        int numBytes = write(socket.m_SocketFD, data, size);

        if (numBytes < 0)
        {
            std::cout << "Error writing to socket" << std::endl;
            return false;
        }

        return true;
    }

    void Socket::Close()
    {
        if (IsValid())
            close(m_SocketFD);
    }
}