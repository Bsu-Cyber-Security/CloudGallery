#include "filetransfermanager.h"

#include <QFileInfo>
#include <QMimeDatabase>
#include <QJsonDocument>

FileTransferManager::FileTransferManager(QTcpSocket* socket, QObject* parent)
    : QObject(parent), tcpSocket(socket)
{
    connect(tcpSocket, &QTcpSocket::readyRead, this, &FileTransferManager::onReadyRead);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &FileTransferManager::onErrorOccurred);
}

FileTransferManager::~FileTransferManager() {}

QString FileTransferManager::encodeFileToBase64(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return "";
    }
    QByteArray fileData = file.readAll();
    file.close();

    QFileInfo fileInfo(filePath);
    QMimeDatabase mimeDatabase;
    QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileInfo);
    QString mime = mimeType.name();

    QJsonObject metadata;
    metadata["filename"] = fileInfo.fileName();
    metadata["mime_type"] = mime;
    metadata["path"] = "";

    QJsonObject data;
    data["metadata"] = metadata;
    data["data"] = QString(fileData.toBase64());

    QJsonDocument doc(data);
    return QString(doc.toJson(QJsonDocument::Compact));
}

void FileTransferManager::sendFile(const QString& sender, const QString& receiver, const QString& filePath, const QString& relativePath) {
    QString base64Data = encodeFileToBase64(filePath);
    if (base64Data.isEmpty()) {
        emit fileTransferFailed("Failed to encode file.");
        return;
    }

    QJsonObject fileObj;
    fileObj["type"] = "file";
    fileObj["sender"] = sender;
    fileObj["receiver"] = receiver;

    QJsonObject fileData;
    QFileInfo fileInfo(filePath);
    fileData["filename"] = fileInfo.fileName();
    fileData["mime_type"] = QString(QMimeDatabase().mimeTypeForFile(fileInfo).name());
    fileData["path"] = relativePath;
    fileData["data"] = base64Data;

    fileObj["file"] = fileData;

    sendData(fileObj);
}

void FileTransferManager::sendImage(const QString& sender, const QString& receiver, const QString& imagePath, const QString& relativePath) {
    QString base64Data = encodeFileToBase64(imagePath);
    if (base64Data.isEmpty()) {
        emit fileTransferFailed("Failed to encode image.");
        return;
    }

    QJsonObject imageObj;
    imageObj["type"] = "image";
    imageObj["sender"] = sender;
    imageObj["receiver"] = receiver;

    QJsonObject imageData;
    QFileInfo fileInfo(imagePath);
    imageData["filename"] = fileInfo.fileName();
    imageData["mime_type"] = QString(QMimeDatabase().mimeTypeForFile(fileInfo).name());
    imageData["path"] = relativePath;
    imageData["data"] = base64Data;

    imageObj["image"] = imageData;

    sendData(imageObj);
}

void FileTransferManager::sendData(const QJsonObject& data) {
    QJsonDocument doc(data);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

    QByteArray packet;
    quint32 size = jsonData.size();
    packet.append(reinterpret_cast<const char*>(&size), sizeof(quint32));
    packet.append(jsonData);

    tcpSocket->write(packet);
}

void FileTransferManager::onReadyRead() {
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
            QString status = response["status"].toString();
            QString message = response["message"].toString();

            if (status == "success") {
                emit fileTransferSuccess(message);
            } else {
                emit fileTransferFailed(message);
            }
        } else {
            qWarning() << "Invalid JSON response received.";
        }
    }
}

void FileTransferManager::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    qWarning() << "Socket error:" << tcpSocket->errorString();
    emit errorOccurred(tcpSocket->errorString());
}
