#include "messageclient.h"
#include "qjsondocument.h"

#include <QBuffer>

MessageClient::MessageClient(const QString& host, const quint16 port, QObject* parent): QObject(parent),
    tcpSocket(new QTcpSocket(this)), serverHost(host), server_port(port)
{
    connect(tcpSocket, &QTcpSocket::connected, this, &MessageClient::onConnected);
    connect(tcpSocket, &QTcpSocket::disconnected,this, &MessageClient::onDisconnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &MessageClient::onReadyRead);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &MessageClient::onErrorOccured);
}

MessageClient::~MessageClient(){
    disconnectFromServer();
}

void MessageClient::connectToServer(){
    if(tcpSocket->state() == QAbstractSocket::UnconnectedState){
        tcpSocket->connectToHost(serverHost, server_port);
    }
}

void MessageClient::disconnectFromServer(){
    tcpSocket->disconnectFromHost();
}

void MessageClient::sendTextMessage(const QString& sender, const QString& reciever, const QString& text){
    QJsonObject message;
    message["type"] = "text";
    message["sender"] = sender;
    message["receiver"] = reciever;
    message["text"] = text;
    sendMessage(message);
}

void MessageClient::sendImageMessage(const QString& sender, const QString& receiver, const QImage& image){
    QByteArray imageData;
    QBuffer buffer(&imageData);

    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QJsonObject message;
    message["type"] = "image";
    message["sender"] = sender;
    message["receiver"] = receiver;
    message["text"] = QString(imageData.toBase64());
    sendMessage(message);
}

void MessageClient::sendMessage(const QJsonObject& message){
    QJsonDocument doc(message);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    QByteArray packet;
    quint32 size = data.size();
    packet.append(reinterpret_cast<const char*>(&size), sizeof(quint32));
    packet.append(data);
    tcpSocket->write(packet);
}

void MessageClient::onConnected(){
    qDebug() << "Connected";
    emit connected();
}

void MessageClient::onDisconnected(){
    qDebug() << "Disconnected";
    emit disconnected();
}

void MessageClient::onReadyRead(){
    buffer.append(tcpSocket->readAll());
    processData();
}

void MessageClient::processData(){
    while(buffer.size() >= sizeof(quint32)){
        quint32 messageSize;
        memcpy(&messageSize, buffer.constData(),sizeof(quint32));

        if(buffer.size() < sizeof(quint32) + messageSize){
            break;
        }

        QByteArray messageData = buffer.mid(sizeof(quint32), messageSize);
        buffer.remove(0, sizeof(quint32) + messageSize);
        QJsonDocument doc = QJsonDocument::fromJson(messageData);

        if(doc.isObject()){
            emit messageReceived(doc.object());
        }
        else{
            qWarning() << "Invalid JSON data recieved";
        }
    }
}

void MessageClient::onErrorOccured(QAbstractSocket::SocketError socketError){
    Q_UNUSED(socketError);
    qWarning() << "Socket Error: " << tcpSocket->errorString();
    emit errorOccurred(tcpSocket->errorString());
}
