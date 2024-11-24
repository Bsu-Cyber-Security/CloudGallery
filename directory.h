// Directory.h
#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <QString>
#include <QVector>
#include "File.h"

class Directory {
public:
    Directory(const QString& name, Directory* parent = nullptr);
    ~Directory();

    QString getName() const;
    QString getPath() const;

    void addSubDirectory(Directory* dir);
    void removeSubDirectory(const QString& name);
    Directory* getSubDirectory(const QString& name) const;

    void addFile(File* file);
    void removeFile(const QString& fileName);
    File* getFile(const QString& fileName) const;

    QVector<Directory*> getSubDirectories() const;
    QVector<File*> getFiles() const;

private:
    QString name;
    Directory* parent;
    QVector<Directory*> subDirectories;
    QVector<File*> files;
};

#endif // DIRECTORY_H
