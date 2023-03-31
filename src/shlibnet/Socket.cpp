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
        : m_Protocol(protocol)
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
    }

    Socket &Socket::operator=(Socket other) {
        std::swap(m_SocketFD, other.m_SocketFD);
        std::swap(m_Protocol, other.m_Protocol);
        return *this;
    }

    bool Socket::Listen(int port) const
    {
        sockaddr_in service{};
        service.sin_family = AF_INET;
        service.sin_port = htons(port);
        service.sin_addr.s_addr = INADDR_ANY;

        if (bind(m_SocketFD, (struct sockaddr*) &service, sizeof(service)) < 0)
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
        sockaddr_in service{};
        hostent* server = gethostbyname(address);

        if (!server)
            return false;

        service.sin_family = AF_INET;
        service.sin_port = htons(port);
        bcopy((char *)server->h_addr,
              (char *)&service.sin_addr.s_addr,
              server->h_length);

        memcpy(m_Address, &service, sizeof(service));

        if (m_Protocol == Protocol::UDP)
            return true;

        if (connect(m_SocketFD, m_Address, sizeof(service)) < 0)
            return false;

        return true;
    }

    int Socket::Receive(void* buffer, int size, Socket* socket) const
    {
        if (socket && m_Protocol != socket->m_Protocol)
            return 0;

        switch(m_Protocol)
        {
            case TCP:
                if (!socket)
                    return (int)recv(m_SocketFD, buffer, size, 0);
                return (int)recv(socket->m_SocketFD, buffer, size, 0);
            case UDP:
                if (!socket)
                    return (int)recvfrom(m_SocketFD, buffer, size, MSG_WAITALL, nullptr, nullptr);
                return (int)recvfrom(m_SocketFD, buffer, size, MSG_WAITALL, socket->m_Address, &socket->m_AddressLength);
            default:
                return 0;
        }
    }

    bool Socket::Send(const Socket& socket, const void* data, int size) const
    {
        int numBytes = (int)write(socket.m_SocketFD, data, size);

        if (numBytes < 0)
            return false;

        return true;
    }

    void Socket::Close()
    {
        if (IsValid())
            close(m_SocketFD);
        m_SocketFD = -1;
    }
}
