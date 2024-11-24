#ifndef SEGMENTMANAGER_H
#define SEGMENTMANAGER_H

#include <unordered_map>
#include "Segment.h"

class SegmentManager {
public:
    SegmentManager();
    ~SegmentManager();

    void createSegment(uint32_t segmentId);
    void deleteSegment(uint32_t segmentId);
    Segment* getSegment(uint32_t segmentId) const;

private:
    std::unordered_map<uint32_t, Segment*> segments;
};

#endif
