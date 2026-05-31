#include "Preprocessor.h"
using namespace std;

ImageData Preprocessor::resize(const ImageData& input, int newWidth, int newHeight)
{
    ImageData newImage;
    unsigned int oldWidth = input.width;
    unsigned int oldHeight = input.height;
    newImage.width = newWidth;
    newImage.height = newHeight;
    newImage.colors = input.colors;

    unsigned int bytesNeeded = newWidth * newHeight * newImage.colors;
    newImage.pixels.resize(bytesNeeded, 0);

    float X_scaling_factor = (newWidth != 0) ? (static_cast<float>(oldWidth) / newWidth) : 0.0f;
    float Y_scaling_factor = (newHeight != 0) ? (static_cast<float>(oldHeight) / newHeight) : 0.0f;

    for (int y = 0; y < newHeight; ++y)
    {
        for (int x = 0; x < newWidth; ++x)
        {
            // map the coordinates back to the original image
            float oldX = (x + 0.5f) * X_scaling_factor - 0.5f;
            float oldY = (y + 0.5f) * Y_scaling_factor - 0.5f;

            float srcX = std::max(0.0f, std::min(oldX, static_cast<float>(input.width - 1)));
            float srcY = std::max(0.0f, std::min(oldY, static_cast<float>(input.height - 1)));

            // calculate the coordinate values for 4 surrounding pixels
            int x1 = static_cast<int>(srcX);
            int y1 = static_cast<int>(srcY);
            int x2 = (x1 + 1 < input.width) ? x1 + 1 : x1;
            int y2 = (y1 + 1 < input.height) ? y1 + 1 : y1;

            // fractional offsets - always in [0, 1), weights always sum to 1
            // even when x2==x1 or y2==y1 (edge pixels)
            float dx = oldX - static_cast<float>(x1);
            float dy = oldY - static_cast<float>(y1);

            for (int c=0; c < newImage.colors; ++c)
            {
                // image starts in the left top, which is counterintuitive
                float q11 = input.pixels[(y1 * input.width + x1) * input.colors + c];
                float q21 = input.pixels[(y1 * input.width + x2) * input.colors + c];
                float q12 = input.pixels[(y2 * input.width + x1) * input.colors + c];
                float q22 = input.pixels[(y2 * input.width + x2) * input.colors + c];

                float f = (1.0f - dx) * (1.0f - dy) * q11 +
                          dx * (1.0f - dy) * q21 +
                          (1.0f - dx) * dy * q12 +
                          dx * dy * q22;

                // using unsigned int would be a waste of memory and a potential issue
                newImage.pixels[(y * newImage.width + x) * newImage.colors + c] = static_cast<unsigned char>(f);
            }
        }
    }
    return newImage;
}

std::vector<float> Preprocessor::normalize(const ImageData& input)
{
    size_t imageSize = static_cast<size_t>(input.width) * input.height;
    unsigned int vectorSize = input.height * input.width * input.colors;
    std::vector<float> normalizedImages(vectorSize);

    // const float mean[] = {0.485f, 0.456f, 0.406f};
    // const float std[] = {0.229f, 0.224f, 0.225f};

    for (size_t y = 0; y < input.height; ++y)
    {
        for (size_t x = 0; x < input.width; ++x)
        {
            size_t srcIdx = (y * input.width + x) * input.colors;
            size_t pixelPos = y * input.width + x;

            // normalizedImages[0 * imageSize + pixelPos] = static_cast<float>(input.pixels[srcIdx]) / 255.0f;
            // normalizedImages[1 * imageSize + pixelPos] = static_cast<float>(input.pixels[srcIdx + 1]) / 255.0f;
            // normalizedImages[2 * imageSize + pixelPos] = static_cast<float>(input.pixels[srcIdx + 2]) / 255.0f;

            float r = static_cast<float>(input.pixels[srcIdx]) / 255.0f;
            float g = static_cast<float>(input.pixels[srcIdx + 1]) / 255.0f;
            float b = static_cast<float>(input.pixels[srcIdx + 2]) / 255.0f;

            normalizedImages[0 * imageSize + pixelPos] = (r - 0.485f) / 0.229f;
            normalizedImages[1 * imageSize + pixelPos] = (g - 0.456f) / 0.224f;
            normalizedImages[2 * imageSize + pixelPos] = (b - 0.406f) / 0.225f;
        }
    }
    return normalizedImages;
}

ProcessedData Preprocessor::process(const ImageData& input, int newWidth, int newHeight)
{
    ImageData resizedImage = resize(input, newWidth, newHeight);

    ProcessedData finalImage;
    finalImage.width = newWidth;
    finalImage.height = newHeight;
    finalImage.normalizedPixels = normalize(resizedImage);

    return finalImage;
}