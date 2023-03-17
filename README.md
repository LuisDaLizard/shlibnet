# ShlibNet - facade of linux/unix sockets

### Description
A c++ library to allow for easier use of sockets on the linux/unix platform.

### Note
As of right now, this library is missing some important features, such as Windows socket support and for using different protocols other than TCP. 

## Multithreading
Many of the methods in the Socket class are blocking so it's recommended to create a multithreaded implementation of a Server and Client. 

One such way to go about making a multithreaded Server using this library is to create a main thread for the server to accept connections and a new thread for each client that is connected to the server. Each client thread on the server should be waiting for messages to be received from a client. 

One idea for implementing a multithreaded client is to just create one extra thread to wait for messages to be received from the server.

## Quickstart
### Server
```cpp
#include <shlibnet/Socket.h>

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
  
  // ReceiveFrom() will wait a message from the specified socket 
  // and set the buffer passed in to the message received and 
  // return the number of bytes of the message.
  char buffer[256];
  int numBytes = server.ReceiveFrom(client, buffer, 256);
  
  // numBytes will be <= 0 if the connection is interupted.
  if (numBytes <= 0)
    return 1;
  
  // Sends a message to the connected client. Requires the number of 
  // bytes being sent.
  const char* message = "Hello!";
  server.SendTo(client, message, 6);
  
  // Will close the server socket and terminate any connections with 
  // clients.
  server.Close();
  
  return 0;
}
```

### Client
```cpp
#include <shlibnet/Socket.h>

using namespace shlib;

int main()
{
  // Create a socket using the TCP protocol
  Socket client(Protocol::TCP);
  
  // Connects to the specified address and port. Returns 
  // false if the connection failed
  const char* address = "127.0.0.1";
  if (!client.Connect(address, 25565))
    return 1;
  
  // Passing the client socket as the socket to send a message 
  // to will send the message to the server.
  const char* message = "Client Hello!"
  client.SendTo(client, message, 13);
  
  // ReceiveFrom() will wait until a message is sent from the given 
  // socket. To receive a message from the connected server, you need
  // pass the client socket this is called on.
  char buffer[256];
  int numBytes = client.ReceiveFrom(client, buffer, 256);
  
  // If the number of bytes returned is <= 0, the connection
  // was interupted.
  if (numBytes <= 0)
    return 1;
  
  // Closing the client socket will terminate all connections
  client.Close();
  
  return 0;
}
```
