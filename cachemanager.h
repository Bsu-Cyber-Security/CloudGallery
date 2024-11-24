#ifndef CACHEMANGER_H
#define CACHEMANAGER_H

#include <unordered_map>
#include <list>
#include "Page.h"

class CacheManager {
public:
    CacheManager(size_t capacity);
    ~CacheManager();

    Page* getPage(uint32_t virtualPage);
    void putPage(uint32_t virtualPage, Page* page);

private:
    size_t capacity;
    std::list<uint32_t> lruList; // Список для реализации алгоритма LRU
    std::unordered_map<uint32_t, std::pair<Page*, std::list<uint32_t>::iterator>> cacheMap;
};

#endif
