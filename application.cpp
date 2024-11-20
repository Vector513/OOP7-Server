#include "application.h"
#include <QDebug>
#include <sstream>

Application::Application(int &argc, char **argv, TcpServer *otherServer, quint16 port)
    : QCoreApplication(argc, argv), server(otherServer)
{
    server->startServer(port);
    connect(server, &TcpServer::messageReceived, this, &Application::onMessageReceived);
}

Application::~Application() {};

void Application::onMessageReceived(QTcpSocket *clientSocket, const QByteArray &message)
{
    qDebug() << "Получено сообщение от клиента:" << message;
    QString messageStr = QString::fromUtf8(message);
    processMessage(clientSocket, messageStr);
}

void Application::processMessage(QTcpSocket *clientSocket, const QString &message)
{
    QString response = "";
    std::istringstream stream(message.toStdString());
    std::stringstream ss;
    std::string command;
    stream >> command;

    if (command == "changeDouble") {
        isComplexMode = false;
        response += "changedModeToDouble";
        ss << '\n';
        polynomDouble.show(ss);
        polynomDouble.show(ss, 0);
        qDebug() << ss.str();
        response += QString::fromStdString(ss.str());
    }
    else if (command == "changeComplex") {
        isComplexMode = true;
        response += "changedModeToComplex";
        ss << '\n';
        polynomComplex.show(ss);
        polynomComplex.show(ss, 0);
        qDebug() << ss.str();
        response += QString::fromStdString(ss.str());
    }

    else if (command == "changeAn") {
        double realPart, imagPart;
        stream >> realPart >> imagPart;
        polynomComplex.setAn(number(realPart, imagPart));
        polynomDouble.setAn(realPart);
    }
    else if (command == "addRoot") {
        double realPart, imagPart;
        stream >> realPart >> imagPart;
        polynomComplex.addRoot(number(realPart, imagPart));
        polynomDouble.addRoot(realPart);
    }
    else if (command == "changeRoot") {
        double realPart, imagPart;
        int index;
        stream >> realPart >> imagPart >> index;
        polynomComplex.setRoot(index, number(realPart, imagPart));
        polynomDouble.setRoot(index, realPart);
    }
    else if (command == "rootsResize") {
        size_t newSize;
        stream >> newSize;
        polynomComplex.resize(newSize);
        polynomDouble.resize(newSize);
    }

    if (isComplexMode) {
        if (command == "changeAn") {
            polynomComplex.show(ss);
            polynomComplex.show(ss, 0);
            response += QString::fromStdString(ss.str());
        }
        else if (command == "addRoot") {
            polynomComplex.show(ss);
            polynomComplex.show(ss, 0);
            response += QString::fromStdString(ss.str());
        }
        else if (command == "changeRoot") {
            polynomComplex.show(ss);
            polynomComplex.show(ss, 0);
            response += QString::fromStdString(ss.str());
        }
        else if (command == "rootsResize") {
            polynomComplex.show(ss);
            polynomComplex.show(ss, 0);
            response += QString::fromStdString(ss.str());
        }
        else if (command == "evaluate") {
            double realPart, imagPart;
            stream >> realPart >> imagPart;
            number res = polynomComplex.evaluate(number(realPart, imagPart));
            response += "p(";
            if (realPart != 0.0 || imagPart != 0.0) {
                if (realPart != 0.0) {
                    response += QString::number(realPart);
                }
                if (imagPart != 0.0) {
                    if (imagPart > 0.0 && realPart != 0.0 ) response += " + ";
                    response += QString::number(imagPart) + 'i';
                }
            }
            else {
                response += QString::number(realPart);
            }
            response += ") = ";
            if (res.getRe() != 0 || res.getIm() != 0) {
                if (res.getRe() != 0) {
                    response += QString::number(res.getRe());
                }
                if (res.getIm() != 0) {
                    if (res.getIm() > 0) response += " + ";
                    else response += " - ";
                    response += QString::number(std::abs(res.getIm())) + "i";
                }
            }
            else {
                response += "0";
            }
        }
    }
    else {
        if (command == "changeAn") {
            polynomDouble.show(ss);
            polynomDouble.show(ss, 0);
            response += QString::fromStdString(ss.str());
        }
        else if (command == "addRoot") {
            polynomDouble.show(ss);
            polynomDouble.show(ss, 0);
            response += QString::fromStdString(ss.str());
        }
        else if (command == "changeRoot") {
            polynomDouble.show(ss);
            polynomDouble.show(ss, 0);
            response += QString::fromStdString(ss.str());
        }
        else if (command == "rootsResize") {
            polynomDouble.show(ss);
            polynomDouble.show(ss, 0);
            response += QString::fromStdString(ss.str());
        }
        else if (command == "evaluate") {
            double realPart;
            stream >> realPart;
            number res = polynomDouble.evaluate(realPart);
            response += "p(";
            response += QString::number(realPart);
            response += ") = ";
            response += QString::number(res.getRe());
        }
    }

    server->sendMessage(clientSocket, response);
}
