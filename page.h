// Page.h
#ifndef PAGE_H
#define PAGE_H

#include <cstdint>

class Page {
public:
    Page(uint32_t pageNumber, size_t size = 4096);
    ~Page();

    uint32_t getPageNumber() const;
    uint8_t* getData();
    const uint8_t* getData() const;
    size_t getSize() const;

private:
    uint32_t pageNumber;
    size_t size;
    uint8_t* data;
};

#endif // PAGE_H
