// MemoryManager.h
#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "PageTable.h"
#include "SegmentManager.h"
#include "CacheManager.h"
#include "Page.h"
#include <QString>
#include <QImage>

class MemoryManager {
public:
    MemoryManager(size_t cacheCapacity, const QString& storageDir);
    ~MemoryManager();

    QImage loadImage(const QString& filePath, uint32_t segmentId);

    void unloadImage(uint32_t virtualPage, uint32_t segmentId);

    Page* loadPage(uint32_t virtualPage, uint32_t segmentId);
    void unloadPage(uint32_t virtualPage, uint32_t segmentId);

private:
    PageTable pageTable;
    SegmentManager segmentManager;
    CacheManager cacheManager;
    QString storageDirectory;

    uint32_t getVirtualPageForImage(const QString& filePath);
    bool readPageFromDisk(uint32_t virtualPage, Page* page);
    bool writePageToDisk(uint32_t virtualPage, Page* page);
    void initializeStorageDirectory();
};

#endif // MEMORYMANAGER_H
