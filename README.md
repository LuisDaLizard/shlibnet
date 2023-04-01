# ShlibNet - facade of linux/unix sockets
[![CMake](https://github.com/LuisDaLizard/shlibnet/actions/workflows/cmake.yml/badge.svg)](https://github.com/LuisDaLizard/shlibnet/actions/workflows/cmake.yml) [![CodeQL](https://github.com/LuisDaLizard/shlibnet/actions/workflows/codeql.yml/badge.svg)](https://github.com/LuisDaLizard/shlibnet/actions/workflows/codeql.yml)
### Description
A c++ library to allow for easier use of sockets on the linux/unix platform.

### Note
As of right now, this library is missing some important features, such as Windows socket support and for using different protocols other than TCP. 

## Multithreading
Many of the methods in the Socket class are blocking so it's recommended to create a multithreaded implementation of a Server and Client. 

One such way to go about making a multithreaded Server using this library is to create a main thread for the server to accept connections and a new thread for each client that is connected to the server. Each client thread on the server should be waiting for messages to be received from a client. 

One idea for implementing a multithreaded client is to just create one extra thread to wait for messages to be received from the server.

## Quickstart
### TCPServer
```cpp
#include <shlibnet/Socket.h>
#include <iostream>

using namespace shlib;

int main()
{
    // Create a socket using the TCP protocol.
    Socket server(Protocol::TCP);

    // Start listening for connections on the specified port.
    server.Listen(25565);

    // Accept() is blocking and will wait for a connection.
    Socket client = server.Accept();

    // If Accept() fails, an invalid Socket will be returned.
    if (!client.IsValid())
        return 1;

    // Receive() will wait a message from the specified socket
    // and set the buffer passed in to the message received and
    // return the number of bytes of the message.
    char buffer[256];
    int numBytes = server.Receive(buffer, 256, &client);

    // numBytes will be <= 0 if the connection is interupted.
    if (numBytes <= 0)
        return 1;

    std::cout << "Message received from client: " << buffer;

    // Sends a message to the connected client. Requires the number of
    // bytes being sent.
    const char* message = "Server Hello!";
    server.Send(message, 13, &client);

    // Will close the server socket and terminate any connections with
    // clients.
    server.Close();

    return 0;
}
```

### TCPClient
```cpp
#include <shlibnet/Socket.h>
#include <iostream>

using namespace shlib;

int main()
{
    // Create a socket using the TCP protocol
    Socket client(Protocol::TCP);

    // Connects to the specified address and port. Returns
    // false if the connection failed
    const char* address = "127.0.0.1"; // "localhost" will also work
    if (!client.Connect(address, 25565))
        return 1;

    // Passing the client socket as the socket to send a message
    // to will send the message to the server.
    const char* message = "Client Hello!";
    client.Send(message, 13);

    // Receive() will wait until a message is sent from the given
    // socket. To receive a message from the connected server, you need
    // pass the client socket this is called on.
    char buffer[256];
    int numBytes = client.Receive(buffer, 256);

    // If the number of bytes returned is <= 0, the connection
    // was interupted.
    if (numBytes <= 0)
        return 1;

    // Print the message received from the server
    std::cout << "Message received from server: " << buffer;

    // Closing the client socket will terminate all connections
    client.Close();

    return 0;
}
```
