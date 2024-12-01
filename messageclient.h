// MessageClient.h
#ifndef MESSAGECLIENT_H
#define MESSAGECLIENT_H

#include <QObject>
#include <QTcpSocket>>
#include <QJsonObject>
#include "Message.h"

class MessageClient : public QObject {
    Q_OBJECT
public:
    explicit MessageClient(const QString& host, const quint16 port, QObject* parent = nullptr);
    ~MessageClient();

    void connectToServer();
    void disconnectFromServer();
    void sendTextMessage(const QString& sender, const QString& reciever, const QString& text);
    void sendImageMessage(const QString& sender, const QString& reciever, const QImage& image);

signals:
    void connected();
    void disconnected();
    void messageReceived(const QJsonObject& message);
    void errorOccurred(const QString& error);

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString& message);
    void onReadOnly();
    void onReadyRead();
    void onErrorOccured(QAbstractSocket::SocketError error);
    void onError();

private:
    QTcpSocket* tcpSocket;
    QString serverHost;
    quint16 server_port;
    QByteArray buffer;
    void processData();
    void sendMessage(const QJsonObject& message);
};

#endif
