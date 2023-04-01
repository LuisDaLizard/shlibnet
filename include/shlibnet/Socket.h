//
// Created by Luis Tadeo Sanchez on 3/6/23.
//

#ifndef SHLIBNET_SOCKET_H
#define SHLIBNET_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>

#include "Protocol.h"

namespace shlib
{
    /**
     * Facade of the linux/unix networking for simpler use of sockets. Supports TCP and UDP
     * sockets
     */
    class Socket {
    private:
        int m_SocketFD;
        sockaddr_in m_Address;
        Protocol m_Protocol;

    public:
        /**
         * Creates a socket using the protocol passed in. Use None for an invalid socket
         *
         * @param protocol The protocol to use for creating the socket.
         */
        explicit Socket(Protocol protocol = Protocol::NONE);

        /**
         * Creates a copy of a socket of which both sockets hold the same socket descriptor.
         *
         * @param other The socket to copy.
         */
        Socket(const Socket& other);

        /**
         * Copies the socket's fields.
         *
         * @param other The socket to copy.
         * @return  The resulting socket
         */
        Socket& operator=(Socket other);

        /**
         * A server only method which binds the socket and listens on the given port.
         *
         * @param port The port to listen on
         * @param backlog
         * @return
         */
        bool Listen(int port, int backlog = 5);

        /**
         * A server only method that accepts an incoming connection. This returns an
         * invalid socket if the protocol does not support accepting connections. If
         * using a UDP socket, do not use this method to accept connections.
         *
         * @return The accepted socket.
         */
        Socket Accept() const;

        /**
         * A client only method that will connect to a given server address and port.
         *
         * @param address The server address.
         * @param port The server port.
         * @return Whether the given address and port are valid.
         */
        bool Connect(const char* address, int port);

        /**
         * This method can be used by both server and client to receive a packet
         * from the given socket.
         *
         * @param socket The socket to expect data from.
         * @param buffer A buffer that will hold the data received.
         * @param bufferSize The size of the buffer.
         * @return The size of the actual data received.
         */
        int Receive(void* buffer, int bufferSize, Socket* socket = nullptr) const;

        /**
         * Sends the data passed in to the socket.
         *
         * @param socket The socket to send the data to.
         * @param data The bytes that should be sent.
         * @param size The size of the data.
         * @return Whether the data was sent successfully.
         */
        bool Send(const void* data, int size, const Socket* socket = nullptr) const;

        /**
         * Closes the socket which closes any connections to the socket
         */
        void Close();

        /**
         * Checks whether the socket is valid. \n\n NOTE: Does not check if the current connection is valid.
         *
         * @return Whether the socket is valid
         */
        [[nodiscard]] inline bool IsValid() const { return m_SocketFD != -1; }
    };
}


#endif //SHLIBNET_SOCKET_H
