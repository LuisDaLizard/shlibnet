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

        bool Listen(int port, int maxConnections);
        Socket Accept();

        bool Connect(const char* address, int port);

        bool ReceiveFrom(Socket socket, char* buffer, int bufferSize);
        bool SendTo(Socket socket, char* data, int size);

        void Close();
        inline bool IsValid() const { return m_SocketFD != -1; }
    };
}


#endif //SHLIBNET_SOCKET_H
