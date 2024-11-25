// Message.h
#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QImage>
#include <QJsonObject>
#include <QCryptographicHash>

class Message {
public:
    Message();
    Message(const QString& sender, const QString& receiver, const QString& text, const QImage& image = QImage());
    ~Message();

    QString getSender() const;
    QString getReceiver() const;
    QString getText() const;
    QImage getImage() const;
    QString getHash() const;
    QJsonObject toJson() const;
    static Message fromJson(const QJsonObject& obj);

private:
    QString sender;
    QString receiver;
    QString text;
    QImage image;
    QString hash;

    void computeHash();
};

#endif // MESSAGE_H
