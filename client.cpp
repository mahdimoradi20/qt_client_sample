#include "client.h"

Client::Client(QObject *parent)
    : QObject{parent}
{

    connect(&socket , &QTcpSocket::connected , this , &Client::connected);
    connect(&socket , &QTcpSocket::disconnected , this , &Client::disconnected);
    connect(&socket , &QTcpSocket::readyRead , this , &Client::readyRead);


}

void Client::start(QString host, quint16 port)
{
    socket.connectToHost(host , port);
    if(!socket.isOpen()){
        qInfo() << socket.errorString();
    }
}


void Client::disconnect()
{
    socket.disconnectFromHost();

}

void Client::connected()
{
    emit success();
}

void Client::disconnected()
{

}

void Client::readyRead()
{

    QJsonDocument document = QJsonDocument::fromJson(socket.readAll());

    emit data_recived(document);

}

void Client::send(QVariantMap map)
{
    QJsonDocument document = QJsonDocument::fromVariant(map);
    socket.write(document.toJson());

}



