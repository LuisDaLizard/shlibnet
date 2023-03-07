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
        : m_SocketFD(-1)
    {

    }

    Socket::Socket(int protocol)
    {
        m_SocketFD = socket(AF_INET, SOCK_STREAM, protocol);
        if (m_SocketFD < 0)
            std::cout << "Error creating socket" << std::endl;
    }

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

        return client;
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

    int Socket::ReceiveFrom(Socket socket, char *buffer, int size) const
    {
        int numBytes = (int)read(socket.m_SocketFD, buffer, size);

        return numBytes;
    }

    bool Socket::SendTo(Socket socket, const char *data, int size) const
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