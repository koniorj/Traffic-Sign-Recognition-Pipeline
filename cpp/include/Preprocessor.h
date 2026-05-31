#ifndef TRAFFIC_SIGN_RECOGNITION_PREPROCESSOR_H
#define TRAFFIC_SIGN_RECOGNITION_PREPROCESSOR_H

// In this class, we'll be taking care of resizing the image by using
// Bilinear Interpolation and then standardizing the image to that
// the CNN can process it.

#include "ImageLoader.h"
#include "Types.h"

class Preprocessor
{
    /**
    * @brief Resizes the image to the target value that our model requires.
    * We use Bilinear Interpolation algorithm.
    * */
    static ImageData resize(const ImageData& input, int newWidth, int newHeight);

    static std::vector<float> normalize(const ImageData& input);

    public:
        static ProcessedData process(const ImageData& input, int newWidth, int newHeight);
};


#endif //TRAFFIC_SIGN_RECOGNITION_PREPROCESSOR_H