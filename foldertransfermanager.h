#ifndef FOLDERTRANSFERMANAGER_H
#define FOLDERTRANSFERMANAGER_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QFileInfoList>
#include "FileTransferManager.h"

class FolderTransferManager : public QObject {
    Q_OBJECT
public:
    explicit FolderTransferManager(FileTransferManager* fileManager, QObject* parent = nullptr);
    ~FolderTransferManager();

    void sendFolder(const QString& sender, const QString& receiver, const QString& folderPath);

signals:
    void folderTransferSuccess(const QString& message);
    void folderTransferFailed(const QString& reason);
    void progressUpdated(int current, int total);
    void errorOccurred(const QString& error);

private:
    FileTransferManager* fileTransferManager;

    void traverseAndSend(const QString& sender, const QString& receiver, const QDir& dir, const QString& basePath, int& current, int total);
};

#endif // FOLDERTRANSFERMANAGER_H
