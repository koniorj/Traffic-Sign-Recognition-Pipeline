// We are going to parse raw PPM/BMP files to simulate how an autonomic car would
// save an image from its camera. It wouldn't be in JPG, but rather in a raw state,
// which saves CPU time.
#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

#ifndef TRAFFIC_SIGN_RECOGNITION_IMAGE_LOADER_H
#define TRAFFIC_SIGN_RECOGNITION_IMAGE_LOADER_H

// Data from a PPM file:
// P6 header
// 58 58 width, height
// 255 maxColor
// Ì(Í)Í)Ó0!Õ3%Û7+Ý6)Ú5&Ù5(Ö5)Ò3&Ð2%Î1$Î0#Ï0#Ë0#Ç/"É/!Ì/"Ë/"É.!Ë.!Í.!Ê/"Æ...

struct ImageData
{
    unsigned int width;
    unsigned int height;
    unsigned int colors = 3; // R G B, or BGR for bmp
    std::vector<uint8_t> pixels;
};

class ImageLoader
{
    /**
     * @throws std::runtime_error If the file does not exist or cannot be opened.
     * @throws std::invalid_argument If the file is completely empty (0 bytes).
     */
    static void validateFile(const std::string& filepath);
public:
    /**
     * @brief Loads a binary PPM (P6) image file.
     * * Parses the PPM header to extract dimensions and reads the raw
     * RGB pixel data directly into the ImageData structure.
     *  @throws std::runtime_error Rethrows from validateFile if the file does not exist or cannot be opened.
     *  @throws std::invalid_argument Rethrows from validateFile if the file is completely empty (0 bytes).
     */
    static ImageData loadPPM(const std::string& filepath);

    static ImageData loadBMP(const std::string& filepath);
};


#endif //TRAFFIC_SIGN_RECOGNITION_IMAGE_LOADER_H


// TODO:
// - add a function that takes care of comments in a PPM file
// - implement BMP version