//
// Created by Luis Tadeo Sanchez on 3/6/23.
//

#ifndef SHLIBNET_SOCKET_H
#define SHLIBNET_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace Shlib
{
    class Socket {
    private:
        int m_SocketFD;
    public:
        Socket();
        Socket(int socketfd);
        Socket(const Socket& other);
        Socket& operator=(Socket other);
        ~Socket();

        bool Listen(int port);
        Socket Accept();

        bool Connect(const char* address, int port);

        int ReceiveFrom(Socket socket, char* buffer, int bufferSize) const;
        bool SendTo(Socket socket, const char* data, int size) const;

        void Close();
        inline bool IsValid() const { return m_SocketFD != -1; }
    };
}


#endif //SHLIBNET_SOCKET_H
