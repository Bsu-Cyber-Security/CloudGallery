#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QImage>

class File {
public:
    File(const QString& name, const QString& path);
    ~File();

    QString getName() const;
    QString getPath() const;
    QImage getImage() const;
    void setImage(const QImage& image);

private:
    QString name;
    QString path;
    QImage image;
};

#endif // FILE_H
