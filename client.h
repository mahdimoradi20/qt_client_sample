#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include<QTcpSocket>
#include<QAbstractSocket>
#include<QRandomGenerator>

#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void start(QString host , quint16 port);
    void send(QVariantMap map);
    void disconnect();

signals:
    void success();
    void data_recived(QJsonDocument data);

private slots:
    void connected();
    void disconnected();
    void readyRead();

private:
    QTcpSocket socket;

};

#endif // CLIENT_H
