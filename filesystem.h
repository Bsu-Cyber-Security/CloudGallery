#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "directory.h"
#include "file.h"
#include <QString>
#include <QImage>

class FileSystem
{
public:
    FileSystem();
    ~FileSystem();

    Directory* getRootDirectory() const;
    Directory* createDirectory(const QString& path);
    Directory* getDirectory(const QString& path) const;

    File* createFile(const QString& path, const QString& file_name, const QImage& image);
    File* getFile(const QString& path, const QString& file_name) const;

    bool deleteFile(const QString& path, const QString& file_name);
    bool deleteDirectory(const QString& path);

};

#endif // FILESYSTEM_H
