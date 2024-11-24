#include "Page.h"
#include <cstring>

Page::Page(uint32_t pageNumber, size_t size)
    : pageNumber(pageNumber), size(size), data(new uint8_t[size]) {
    std::memset(data, 0, size);
}

Page::~Page() {
    delete[] data;
}

uint32_t Page::getPageNumber() const {
    return pageNumber;
}

uint8_t* Page::getData() {
    return data;
}

const uint8_t* Page::getData() const {
    return data;
}

size_t Page::getSize() const {
    return size;
}
