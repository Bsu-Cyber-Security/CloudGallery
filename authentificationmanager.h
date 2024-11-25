#ifndef AUTHENTIFICATIONMANAGER_H
#define AUTHENTIFICATIONMANAGER_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

class AuthentificationManager : public QObject
{
    Q_OBJECT
public:
    explicit AuthentificationManager(QObject* parent = nullptr);
    ~AuthentificationManager();

    void login(const QString& username, const QString& password);
    void logout();
    bool isAuthentificated();
    QString getToken() const;

signals:
    void loginSuccess();
    void loginFailed(const QString& reason);
    void tokenExpired();

private slots:
    void onLoginFinished(QNetworkReply* reply);
    void onTokenRefreshTimeout();

private:
    QNetworkAccessManager* networkManager;
    QString token;
    QTimer* tokenRefreshTimer;

    void refreshToken();
};

#endif // AUTHENTIFICATIONMANAGER_H
