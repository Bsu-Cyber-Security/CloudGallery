#include "segmentmanager.h"

SegmentManager::SegmentManager() {}

SegmentManager::~SegmentManager() {
    for(auto& pair : segments) {
        delete pair.second;
    }
}

void SegmentManager::createSegment(uint32_t segmentId) {
    if(segments.find(segmentId) == segments.end()) {
        segments[segmentId] = new Segment(segmentId);
    }
}

void SegmentManager::deleteSegment(uint32_t segmentId) {
    auto it = segments.find(segmentId);
    if(it != segments.end()) {
        delete it->second;
        segments.erase(it);
    }
}

Segment* SegmentManager::getSegment(uint32_t segmentId) const {
    auto it = segments.find(segmentId);
    if(it != segments.end()) {
        return it->second;
    }
    return nullptr;
}
