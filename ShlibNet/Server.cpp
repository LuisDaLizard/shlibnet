//
// Created by Luis Tadeo Sanchez on 3/7/23.
//

#include "Server.h"

namespace Shlib
{

    Server::Server(int maxClients)
    {
        m_Clients = new Socket[maxClients];
        m_ClientThreads = new std::thread[maxClients];
    }

    Server::~Server()
    {
        m_ServerThread.join();

        delete[] m_Clients;
        delete[] m_ClientThreads;
    }

    bool Server::Start(int port)
    {
        m_Server = Socket(IPPROTO_TCP);

        if (!m_Server.Listen(port))
            return false;

        m_ServerThread = std::thread(Server::MainServerFunc, this);

        return true;
    }

    void Server::MainServerFunc(Server *server)
    {
        while (true)
        {
            Socket temp = server->m_Server.Accept();

            if (!temp.IsValid())
                continue;

            int id = -1;

            for (int i = 0; i < server->m_MaxClients; i++)
                if (!server->m_Clients[i].IsValid())
                    id = i;

            if (id == -1)
            {
                temp.Close();
                continue;
            }

            server->m_Clients[id] = temp;

            if (server->m_ClientThreads[id].joinable())
                server->m_ClientThreads[id].join();
            server->m_ClientThreads[id] = std::thread(Server::ClientFunc, server, std::ref(server->m_Clients[id]), id);

            server->OnAccept(server->m_Clients[id], id);
        }
    }

    void Server::ClientFunc(Server *server, Socket &client, int id)
    {
        while(true)
        {
            char buffer[256] {};
            int numBytes = server->m_Server.ReceiveFrom(client, buffer, 256);

            if (numBytes <= 0)
            {
                server->OnDisconnect(client, id);
                client.Close();
                return;
            }

            server->OnReceive(client, id, buffer, numBytes);
        }
    }
}