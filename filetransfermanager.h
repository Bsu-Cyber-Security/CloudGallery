#ifndef FILETRANSFERMANAGER_H
#define FILETRANSFERMANAGER_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QFile>
#include <QJsonObject>

class FileTransferManager : public QObject {
    Q_OBJECT
public:
    explicit FileTransferManager(QTcpSocket* socket, QObject* parent = nullptr);
    ~FileTransferManager();

    void sendFile(const QString& sender, const QString& receiver, const QString& filePath, const QString& relativePath = "");
    void sendImage(const QString& sender, const QString& receiver, const QString& imagePath, const QString& relativePath = "");

signals:
    void fileTransferSuccess(const QString& message);
    void fileTransferFailed(const QString& reason);
    void errorOccurred(const QString& error);

private slots:
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket* tcpSocket;
    QByteArray buffer;

    void sendData(const QJsonObject& data);
    QString encodeFileToBase64(const QString& filePath) const;
};

#endif // FILETRANSFERMANAGER_H
