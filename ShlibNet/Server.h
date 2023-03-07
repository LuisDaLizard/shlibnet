//
// Created by Luis Tadeo Sanchez on 3/7/23.
//

#ifndef SHLIBNET_SERVER_H
#define SHLIBNET_SERVER_H

#include <thread>

#include "Socket.h"

namespace Shlib
{
    class Server {
    private:
        int m_MaxClients;

        Socket m_Server;
        std::thread m_ServerThread;

        Socket* m_Clients;
        std::thread* m_ClientThreads;
    public:
        Server(int maxClients);
        ~Server();

        bool Start(int port);

        bool Send(const char* data, int size);
        bool Send(Socket recipient, const char* data, int size);

    protected:
        virtual void OnAccept(const Socket& client, int id) {};
        virtual void OnReceive(const Socket& client, int id, char* data, int size) {};
        virtual void OnDisconnect(const Socket& client, int id) {};

    private:
        static void MainServerFunc(Server* server);
        static void ClientFunc(Server* server, Socket& client, int id);
    };
}


#endif //SHLIBNET_SERVER_H
