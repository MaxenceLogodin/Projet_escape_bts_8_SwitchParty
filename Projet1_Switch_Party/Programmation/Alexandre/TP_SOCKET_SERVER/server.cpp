#include "server.h"

#include <QtNetwork>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>

//###############################################################################################################
// Constructeur
Server::Server()
    :   m_tcp_server(0), m_network_session(0)
{
    QNetworkConfigurationManager manager;
    QNetworkConfiguration config = manager.defaultConfiguration();
    m_network_session = new QNetworkSession(config, this);

    m_blockSize = 0;//initialisation

    // La méthode sessionOuverte sera appelée sur le signal opened
    connect(m_network_session, SIGNAL(opened()), this, SLOT(sessionOuverte()));

    // Ouverture de la session
    m_network_session->open();



}

//###############################################################################################################
// Méthode appelée lors de l'ouverture de session
void Server::sessionOuverte()
{
    m_tcp_server = new QTcpServer(this); // instanciation

    // La méthode connexionClient sera appelée sur le signal newConnection
    connect(m_tcp_server, SIGNAL(newConnection()), this, SLOT(connexionClient()));

    if (!m_tcp_server->listen(QHostAddress::Any, 53000)) // on écoute sur le port 53000
    {
        std::cout << "Le serveur n'a pas été correctement lancee." << std::endl;
    }
    else
    {
        std::cout << "Le serveur est pret \n\nIP: " << m_tcp_server->serverAddress().toString().toStdString()
                  << std::endl << "port: " << m_tcp_server->serverPort() << std::endl
                  <<  "En attente d'une connexion..." << std::endl;
    }
}

//###############################################################################################################
// Méthode appelée lors de l'ouverture d'une connexion avec un client
void Server::connexionClient()
{
    // instanciation
    m_socket_client = m_tcp_server->nextPendingConnection();
    // La méthode lireTexte sera appelée sur le signal readyRead
    connect(m_socket_client, SIGNAL(readyRead()), this, SLOT(lireTexte()));

}


//###############################################################################################################
// Méthode appelée lors de la réception d'un texte
void Server::lireTexte()
{
    QDataStream in(m_socket_client);
    in.setVersion(QDataStream::Qt_4_0);

    if (m_blockSize == 0) {
        if (m_socket_client->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> m_blockSize;
    }

    if (m_socket_client->bytesAvailable() < m_blockSize)
        return;

    QString texte;
    in >> texte;

    std::cout << texte.toStdString() << std::endl;
    QString nb;
    if (texte.startsWith("GAGNE"))
    {
        QStringList liste = texte.split(":");
        nb = liste.at(1);
    }
    std::cout <<"Mise en base de donnees du nombre: " << nb.toStdString() << std::endl;

    m_blockSize = 0;
}
