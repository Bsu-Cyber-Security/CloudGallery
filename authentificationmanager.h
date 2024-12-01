#ifndef AUTHENTIFICATIONMANAGER_H
#define AUTHENTIFICATIONMANAGER_H

#include <QString>
#include <QTcpSocket>

class AuthentificationManager : public QObject {
    Q_OBJECT
public:
    explicit AuthentificationManager(QTcpSocket* socket, QObject* parent = nullptr);
    ~AuthentificationManager();

    void registerUser(const QString& username, const QString& password);
    void loginUser(const QString& username, const QString& password);
    bool isAuthenticated() const;
    QString getToken() const;

signals:
    void registrationSuccess();
    void registrationFailed(const QString& reason);
    void loginSuccess();
    void loginFailed(const QString& reason);
    void errorOccurred(const QString& error);

private slots:
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket* tcpSocket;
    bool authenticated;
    QString token;
    QByteArray buffer;

    QString hashPassword(const QString& password) const;
    void processResponse(const QJsonObject& response);
};

#endif // AUTHENTIFICATIONMANAGER_H
