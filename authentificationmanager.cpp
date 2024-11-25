#include "authentificationmanager.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

AuthentificationManager::AuthentificationManager(QObject* parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this)), tokenRefreshTimer(new QTimer(this)) {
    connect(networkManager, &QNetworkAccessManager::finished, this, &AuthentificationManager::onLoginFinished);
    connect(tokenRefreshTimer, &QTimer::timeout, this, &AuthentificationManager::onTokenRefreshTimeout);
}

AuthentificationManager::~AuthentificationManager() {}

void AuthentificationManager::login(const QString& username, const QString& password){
    QUrl url("https:/...");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    networkManager->post(request, data);
}

void AuthentificationManager::logout() {
    token.clear();
    tokenRefreshTimer->stop();
    emit loginFailed("Logged out");
}

bool AuthentificationManager::isAuthentificated()  {
    return !token.isEmpty();
}

QString AuthentificationManager::getToken() const {
    return token;
}

void AuthentificationManager::onLoginFinished(QNetworkReply* reply){
    if(reply->error() == QNetworkReply::NoError){
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);

        if(doc.isObject()){
            QJsonObject obj = doc.object();
            if(obj.contains("token")){
                token = obj["token"].toString();
                emit loginSuccess();
                tokenRefreshTimer->start(50 * 60 * 1000);
            }
            else{
                emit loginFailed("Token not found in response");
            }
        }
        else{
            emit loginFailed("Invalid JSON response");
        }
    }
    else{
        emit loginFailed(reply->errorString());
    }

    reply->deleteLater();
}

void AuthentificationManager::refreshToken(){
    if(token.isEmpty()) {
        emit tokenExpired();
        return;
    }

    QUrl url("https:/...");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());

    QJsonObject json;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    networkManager->post(request, data);
}

void AuthentificationManager::onTokenRefreshTimeout() {
    refreshToken();
}
