#include "Segment.h"
#include <algorithm>

Segment::Segment(uint32_t segmentId) : segmentId(segmentId) {}

Segment::~Segment() {}

uint32_t Segment::getSegmentId() const {
    return segmentId;
}

void Segment::addPage(uint32_t virtualPage) {
    pages.push_back(virtualPage);
}

void Segment::removePage(uint32_t virtualPage) {
    pages.erase(std::remove(pages.begin(), pages.end(), virtualPage), pages.end());
}

const std::vector<uint32_t>& Segment::getPages() const {
    return pages;
}
