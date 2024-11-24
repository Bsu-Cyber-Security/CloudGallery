#include "CacheManager.h"

CacheManager::CacheManager(size_t capacity) : capacity(capacity) {}

CacheManager::~CacheManager() {
    for(auto& pair : cacheMap) {
        delete pair.second.first;
    }
}

Page* CacheManager::getPage(uint32_t virtualPage) {
    auto it = cacheMap.find(virtualPage);
    if(it == cacheMap.end()) {
        return nullptr;
    }
    // Переместить страницу в начало списка
    lruList.erase(it->second.second);
    lruList.push_front(virtualPage);
    it->second.second = lruList.begin();
    return it->second.first;
}

void CacheManager::putPage(uint32_t virtualPage, Page* page) {
    auto it = cacheMap.find(virtualPage);
    if(it != cacheMap.end()) {
        // Обновить позицию в LRU списке
        lruList.erase(it->second.second);
        lruList.push_front(virtualPage);
        it->second.second = lruList.begin();
        delete it->second.first;
        it->second.first = page;
    } else {
        if(cacheMap.size() >= capacity) {
            // Удалить наименее недавно использованную страницу
            uint32_t lruPage = lruList.back();
            lruList.pop_back();
            delete cacheMap[lruPage].first;
            cacheMap.erase(lruPage);
        }
        lruList.push_front(virtualPage);
        cacheMap[virtualPage] = {page, lruList.begin()};
    }
}
