//
// Created by Luis Tadeo Sanchez on 3/6/23.
//

#ifndef SHLIBNET_SOCKET_H
#define SHLIBNET_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace shlib
{
    /**
     * Facade of the linux/unix networking for simpler use of sockets. Currently
     * only supports tcp sockets
     */
    class Socket {
    private:
        int m_SocketFD;

    public:
        enum Protocol
        {
            TCP = IPPROTO_TCP,
            //UDP = IPPROTO_UDP,
        };

    public:

        /**
         *  Creates an invalid socket
         */
        Socket();

        /**
         * Creates a socket using the protocol passed in
         *
         * @param protocol
         */
        explicit Socket(Protocol protocol);

        /**
         * Creates a copy of a socket of which both sockets hold the same socket descriptor
         *
         * @param other The socket to copy
         */
        Socket(const Socket& other);

        /**
         *
         *
         * @param other
         * @return
         */
        Socket& operator=(Socket other);

        /**
         * A server only method which binds the socket and listens on the given port.
         *
         * Sets the backlog to 5
         *
         * @param port
         * @return
         */
        bool Listen(int port);

        /**
         * BLOCKING
         *
         * A server only method that accepts an incoming connection.
         *
         * @return The accepted socket
         */
        Socket Accept();

        /**
         * BLOCKING
         *
         * A client only method that will connect to a given server address and port
         *
         * @param address
         * @param port
         * @return
         */
        bool Connect(const char* address, int port);

        /**
         * BLOCKING
         *
         * This method can be used by both server and client to receive a packet
         * from the given socket
         *
         * NOTE:
         *
         * @param socket
         * @param buffer
         * @param bufferSize
         * @return
         */
        int ReceiveFrom(Socket socket, void* buffer, int bufferSize) const;

        /**
         * BLOCKING
         *
         * @param socket
         * @param data
         * @param size
         * @return
         */
        bool SendTo(Socket socket, const void* data, int size) const;

        /**
         * Closes the socket which closes any connections to the socket
         */
        void Close();

        /**
         * Checks whether the socket is valid
         *
         * @return
         */
        inline bool IsValid() const { return m_SocketFD != -1; }
    };
}


#endif //SHLIBNET_SOCKET_H
