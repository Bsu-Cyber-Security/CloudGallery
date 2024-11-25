// Message.cpp
#include "Message.h"
#include <QJsonDocument>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>

Message::Message()
    : sender(""), receiver(""), text(""), image(), hash("") {}

Message::Message(const QString& sender, const QString& receiver, const QString& text, const QImage& image)
    : sender(sender), receiver(receiver), text(text), image(image), hash("") {
    computeHash();
}

Message::~Message() {}

QString Message::getSender() const {
    return sender;
}

QString Message::getReceiver() const {
    return receiver;
}

QString Message::getText() const {
    return text;
}

QImage Message::getImage() const {
    return image;
}

QString Message::getHash() const {
    return hash;
}

void Message::computeHash() {
    QByteArray data;
    data.append(sender.toUtf8());
    data.append(receiver.toUtf8());
    data.append(text.toUtf8());

    if(!image.isNull()) {
        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        data.append(imageData);
    }

    QByteArray computedHash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    hash = QString(computedHash.toHex());
}

QJsonObject Message::toJson() const {
    QJsonObject obj;
    obj["sender"] = sender;
    obj["receiver"] = receiver;
    obj["text"] = text;
    obj["hash"] = hash;

    if(!image.isNull()) {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG"); // Сохраняем в PNG
        QString imageData = QString::fromLatin1(byteArray.toBase64());
        obj["image"] = imageData;
    }
    else {
        obj["image"] = "";
    }

    return obj;
}

Message Message::fromJson(const QJsonObject& obj) {
    QString sender = obj["sender"].toString();
    QString receiver = obj["receiver"].toString();
    QString text = obj["text"].toString();
    QString hashReceived = obj["hash"].toString();
    QString imageData = obj["image"].toString();

    QImage image;
    if(!imageData.isEmpty()) {
        QByteArray byteArray = QByteArray::fromBase64(imageData.toLatin1());
        image.loadFromData(byteArray, "PNG");
    }

    Message message(sender, receiver, text, image);
    message.hash = hashReceived;

    return message;
}
