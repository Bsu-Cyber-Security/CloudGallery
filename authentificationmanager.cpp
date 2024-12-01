#include "authentificationmanager.h".h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QDebug>

AuthentificationManager::AuthentificationManager(QTcpSocket* socket, QObject* parent)
    : QObject(parent), tcpSocket(socket), authenticated(false), token("")
{
    connect(tcpSocket, &QTcpSocket::readyRead, this, &AuthentificationManager::onReadyRead);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &AuthentificationManager::onErrorOccurred);
}

AuthentificationManager::~AuthentificationManager() {}

QString AuthentificationManager::hashPassword(const QString& password) const {
    QByteArray byteArray = password.toUtf8();
    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

void AuthentificationManager::registerUser(const QString& username, const QString& password) {
    QJsonObject request;
    request["action"] = "register";
    request["username"] = username;
    request["password"] = hashPassword(password);

    QJsonDocument doc(request);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    QByteArray packet;
    quint32 size = data.size();
    packet.append(reinterpret_cast<const char*>(&size), sizeof(quint32));
    packet.append(data);

    tcpSocket->write(packet);
}

void AuthentificationManager::loginUser(const QString& username, const QString& password) {
    QJsonObject request;
    request["action"] = "login";
    request["username"] = username;
    request["password"] = hashPassword(password);

    QJsonDocument doc(request);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    QByteArray packet;
    quint32 size = data.size();
    packet.append(reinterpret_cast<const char*>(&size), sizeof(quint32));
    packet.append(data);

    tcpSocket->write(packet);
}

void AuthentificationManager::onReadyRead() {
    buffer.append(tcpSocket->readAll());

    while (buffer.size() >= sizeof(quint32)) {
        quint32 messageSize;
        memcpy(&messageSize, buffer.constData(), sizeof(quint32));

        if (buffer.size() < sizeof(quint32) + messageSize){
            break;
        }

        QByteArray messageData = buffer.mid(sizeof(quint32), messageSize);
        buffer.remove(0, sizeof(quint32) + messageSize);

        QJsonDocument doc = QJsonDocument::fromJson(messageData);
        if (doc.isObject()) {
            QJsonObject response = doc.object();
            processResponse(response);
        } else {
            qWarning() << "Invalid JSON response received.";
        }
    }
}

void AuthentificationManager::processResponse(const QJsonObject& response) {
    QString status = response["status"].toString();
    QString message = response["message"].toString();

    if (response.contains("token"))
        token = response["token"].toString();

    if (status == "success") {
        QString action = response["action"].toString();
        if (action == "register") {
            emit registrationSuccess();
        } else if (action == "login") {
            authenticated = true;
            emit loginSuccess();
        }
    } else {
        QString action = response["action"].toString();
        if (action == "register") {
            emit registrationFailed(message);
        } else if (action == "login") {
            emit loginFailed(message);
        }
    }
}

bool AuthentificationManager::isAuthenticated() const {
    return authenticated;
}

QString AuthentificationManager::getToken() const {
    return token;
}

void AuthentificationManager::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    qWarning() << "Socket error:" << tcpSocket->errorString();
    emit errorOccurred(tcpSocket->errorString());
}
