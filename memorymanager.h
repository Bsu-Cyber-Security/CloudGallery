// MemoryManager.h
#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "PageTable.h"
#include "SegmentManager.h"
#include "CacheManager.h"
#include "Page.h"
#include "FileSystem.h"
#include <QString>
#include <QImage>

class MemoryManager {
public:
    MemoryManager(size_t cacheCapacity, const QString& storageDir);
    ~MemoryManager();

    QImage loadImage(const QString& filePath, const QString& directoryPath);

    void unloadImage(const QString& filePath, const QString& directoryPath);

    Page* loadPage(uint32_t virtualPage, uint32_t segmentId);
    void unloadPage(uint32_t virtualPage, uint32_t segmentId);

    FileSystem* getFileSystem() const;

private:
    PageTable pageTable;
    SegmentManager segmentManager;
    CacheManager cacheManager;
    QString storageDirectory;
    FileSystem* fileSystem;

    uint32_t getVirtualPageForImage(const QString& filePath) const;
    bool readPageFromDisk(uint32_t virtualPage, Page* page);
    bool writePageToDisk(uint32_t virtualPage, Page* page);
    void initializeStorageDirectory();
};

#endif // MEMORYMANAGER_H
