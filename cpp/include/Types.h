#ifndef TYPES_H
#define TYPES_H

#include <vector>

struct ProcessedData {
    int width;
    int height;
    std::vector<float> normalizedPixels;
};

#endif