
#ifndef CLIENT_H
#define CLIENT_H

/**
 * \file client.h
 * \brief Fichier de déclaration de la classe client.
 * \author Alexandre Stanisiere
 */

#include <QTcpSocket>
#include <QObject>

class QTcpSocket;
class QNetworkSession;

/**
 * \class Client
 * \brief Classe décrivant un client. Le client a pour rôle d'envoyer un message au serveur.
 * \author Alexandre Stanisiere
 */

class Client : public QObject
{
    Q_OBJECT

public:
    Client();

private slots:
    void afficherErreur(QAbstractSocket::SocketError socketError);

private:
    /** \brief QTcpSocket fournissant les paramètres nécessaires à l'établiseement d'une connexion avec le serveur sous forme d'objet. */
    QTcpSocket *m_tcpSocket;
    /** \brief QNetworkSession permet de créer une session de communication avec le serveur sous forme d'objet. */
    QNetworkSession *m_networkSession;
        /** \brief Fonction permettant d'envoyer un message au serveur une fois la connexion établie. */
    void envoiTexte( const std::string& s);
};

#endif
