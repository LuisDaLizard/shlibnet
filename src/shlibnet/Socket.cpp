//
// Created by Luis Tadeo Sanchez on 3/6/23.
//

#include "shlibnet/Socket.h"

#include <unistd.h>
#include <netdb.h>
#include <algorithm>
#include <strings.h>

namespace shlib
{
    Socket::Socket(Protocol protocol)
        : m_Protocol(protocol), m_Address()
    {
        switch(m_Protocol)
        {
            case TCP:
                m_SocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                break;
            case UDP:
                m_SocketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                break;
            default:
                m_SocketFD = -1;
                break;
        }
    }

    Socket::Socket(const Socket &other)
    {
        m_SocketFD = other.m_SocketFD;
        m_Protocol = other.m_Protocol;
        m_Address = other.m_Address;
    }

    Socket &Socket::operator=(Socket other) {
        std::swap(m_SocketFD, other.m_SocketFD);
        std::swap(m_Protocol, other.m_Protocol);
        std::swap(m_Address, other.m_Address);
        return *this;
    }

    bool Socket::Listen(int port)
    {
        m_Address.sin_family = AF_INET;
        m_Address.sin_port = htons(port);
        m_Address.sin_addr.s_addr = INADDR_ANY;

        if (bind(m_SocketFD, (struct sockaddr*) &m_Address, sizeof(m_Address)) < 0)
            return false;

        if (m_Protocol == Protocol::TCP)
            listen(m_SocketFD, 5);

        return true;
    }

    Socket Socket::Accept() const
    {
        Socket client;

        if (m_Protocol == Protocol::TCP)
        {
            client.m_SocketFD = accept(m_SocketFD, nullptr, nullptr);
            client.m_Protocol = Protocol::TCP;
        }

        return client;
    }

    bool Socket::Connect(const char *address, int port)
    {
        hostent* server = gethostbyname(address);

        if (!server)
            return false;

        m_Address.sin_family = AF_INET;
        m_Address.sin_port = htons(port);
        bcopy((char *)server->h_addr,
              (char *)&m_Address.sin_addr.s_addr,
              server->h_length);

        if (m_Protocol == Protocol::UDP)
            return true;

        if (connect(m_SocketFD, (struct sockaddr*) &m_Address, sizeof(m_Address)) < 0)
            return false;

        return true;
    }

    int Socket::Receive(void* buffer, int size, Socket* socket) const
    {
        if (socket && m_Protocol != socket->m_Protocol)
            return 0;

        socklen_t addrSize;

        switch(m_Protocol)
        {
            case TCP:
                if (!socket)
                    return (int)recv(m_SocketFD, buffer, size, 0);
                return (int)recvfrom(socket->m_SocketFD, buffer, size, 0, nullptr, nullptr);
            case UDP:
                if (!socket)
                    return (int)recvfrom(m_SocketFD, buffer, size, 0, nullptr, nullptr);
                return (int)recvfrom(m_SocketFD, buffer, size, 0, (sockaddr *)&socket->m_Address, &addrSize);
            default:
                return 0;
        }
    }

    bool Socket::Send(const void* data, int size, const Socket* socket) const
    {
        if (socket && m_Protocol != socket->m_Protocol)
            return false;

        int numBytes = 0;

        switch(m_Protocol)
        {
            case TCP:
                if (!socket)
                    numBytes = (int)send(m_SocketFD, data, size, 0);
                else
                    numBytes = (int)send(socket->m_SocketFD, data, size, 0);

                if (numBytes < 0)
                    return false;

                return true;
            case UDP:
                if (!socket)
                    return false;

                numBytes = (int)sendto(m_SocketFD, data, size, 0, (struct sockaddr*) &socket->m_Address, sizeof(socket->m_Address));

                if (numBytes < 0)
                    return false;

                return true;
            default:
                return false;
        }
    }

    void Socket::Close()
    {
        if (IsValid())
            close(m_SocketFD);
        m_SocketFD = -1;
    }
}
