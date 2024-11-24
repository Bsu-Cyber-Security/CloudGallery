#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <unordered_map>
#include "Page.h"

class PageTable {
public:
    PageTable();
    ~PageTable();

    void mapPage(uint32_t virtualPage, Page* physicalPage);
    Page* getPhysicalPage(uint32_t virtualPage) const;
    void unmapPage(uint32_t virtualPage);

private:
    std::unordered_map<uint32_t, Page*> table;
};

#endif
