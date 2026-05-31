//
// Created by konio on 5/29/2026.
//

#ifndef TRAFFICSIGNRECOGNITION_TYPES_H
#define TRAFFICSIGNRECOGNITION_TYPES_H

#ifndef TYPES_H
#define TYPES_H

#include <vector>

struct ProcessedData {
    int width;
    int height;
    std::vector<float> normalizedPixels;
};

#endif

#endif //TRAFFICSIGNRECOGNITION_TYPES_H