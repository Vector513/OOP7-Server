#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCoreApplication>
#include <QTcpSocket>
#include "tcpServer.h"
#include "polynom.h"
#include "number.h"

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv, TcpServer *otherServer, quint16 port);
    ~Application();

private slots:
    void onMessageReceived(QTcpSocket *clientSocket, const QByteArray &message);

private:
    void processMessage(QTcpSocket *clientSocket, const QString &message);

    TcpServer* server;
    Polynom<number> polynomComplex;
    Polynom<double> polynomDouble;

    bool isComplexMode = false;
};

#endif // APPLICATION_H
