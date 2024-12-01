#include "FolderTransferManager.h"
#include <QDebug>

FolderTransferManager::FolderTransferManager(FileTransferManager* fileManager, QObject* parent)
    : QObject(parent), fileTransferManager(fileManager)
{
    connect(fileTransferManager, &FileTransferManager::fileTransferSuccess, this, &FolderTransferManager::folderTransferSuccess);
    connect(fileTransferManager, &FileTransferManager::fileTransferFailed, this, &FolderTransferManager::folderTransferFailed);
    connect(fileTransferManager, &FileTransferManager::errorOccurred, this, &FolderTransferManager::errorOccurred);
}

FolderTransferManager::~FolderTransferManager() {}

void FolderTransferManager::sendFolder(const QString& sender, const QString& receiver, const QString& folderPath) {
    QDir dir(folderPath);
    if (!dir.exists()) {
        emit folderTransferFailed("Folder does not exist.");
        return;
    }

    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoSymLinks, QDir::DirsFirst);
    int totalFiles = fileList.size();
    if (totalFiles == 0) {
        emit folderTransferFailed("Folder is empty.");
        return;
    }

    int currentFile = 0;
    traverseAndSend(sender, receiver, dir, dir.absolutePath(), currentFile, totalFiles);
}

void FolderTransferManager::traverseAndSend(const QString& sender, const QString& receiver, const QDir& dir, const QString& basePath, int& current, int total) {
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoSymLinks, QDir::DirsFirst);
    foreach (const QFileInfo& fileInfo, fileList) {
        if (fileInfo.isDir()) {
            QDir subDir(fileInfo.absoluteFilePath());
            traverseAndSend(sender, receiver, subDir, basePath, current, total);
        } else {
            QString relativePath = QDir(basePath).relativeFilePath(fileInfo.absoluteFilePath());
            QString filePath = fileInfo.absoluteFilePath();

            // Определяем, является ли файл изображением
            QString suffix = fileInfo.suffix().toLower();
            if (suffix == "png" || suffix == "jpg" || suffix == "jpeg" || suffix == "bmp" || suffix == "gif") {
                fileTransferManager->sendImage(sender, receiver, filePath, relativePath);
            } else {
                fileTransferManager->sendFile(sender, receiver, filePath, relativePath);
            }

            current++;
            emit progressUpdated(current, total);
        }
    }

    if (current == total) {
        emit folderTransferSuccess("Folder transfer completed successfully.");
    }
}
