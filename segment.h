#ifndef SEGMENT_H
#define SEGMENT_H

#include <vector>
#include <cstdint>

class Segment {
public:
    Segment(uint32_t segmentId);
    ~Segment();

    uint32_t getSegmentId() const;
    void addPage(uint32_t virtualPage);
    void removePage(uint32_t virtualPage);
    const std::vector<uint32_t>& getPages() const;

private:
    uint32_t segmentId;
    std::vector<uint32_t> pages;
};

#endif
