#include "PageTable.h"

PageTable::PageTable() {}

PageTable::~PageTable() {
    for(auto& pair : table) {
        delete pair.second;
    }
}

void PageTable::mapPage(uint32_t virtualPage, Page* physicalPage) {
    table[virtualPage] = physicalPage;
}

Page* PageTable::getPhysicalPage(uint32_t virtualPage) const {
    auto it = table.find(virtualPage);
    if(it != table.end()) {
        return it->second;
    }
    return nullptr;
}

void PageTable::unmapPage(uint32_t virtualPage) {
    auto it = table.find(virtualPage);
    if(it != table.end()) {
        delete it->second;
        table.erase(it);
    }
}
