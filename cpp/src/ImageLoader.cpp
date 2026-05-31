#include <cstdint>
#include <fstream>
#include <vector>
#include "ImageLoader.h"

using namespace std;

void ImageLoader::validateFile(const std::string& filepath)
{
    ifstream file(filepath);

    if (!file.is_open()) {
        throw std::runtime_error("ImageLoader: this file does not exist or we do not have the permission to access it: " + filepath);
    }

    if (file.peek() == ifstream::traits_type::eof())
    {
        throw std::invalid_argument("ImageLoader: This file is empty: " + filepath);
    }
}

ImageData ImageLoader::loadPPM(const std::string& filepath)
{
    validateFile(filepath);

    ifstream file(filepath, ios::binary);
    string headerNumber;
    int width=0, height=0, maxColor=0;

    file >> headerNumber >> width >> height >> maxColor;
    // An empty line is left after we get all the variables. Now it's time
    // for the binary data
    file.get();

    if (headerNumber != "P6" || maxColor != 255)
    {
        throw runtime_error("This is not a PPM file.");
    }

    ImageData imageData;
    imageData.width = width;
    imageData.height = height;
    imageData.colors = 3;

    int bytesNeeded = width * height * 3;
    imageData.pixels.resize(bytesNeeded);

    // We read the rest of the file into the imageData.pixels

    // istream& read(char* s, streamsize n); we need to cast to char
    file.read(reinterpret_cast<char*>(imageData.pixels.data()), bytesNeeded);
    // imageData.pixels.data() gives us the physical start of the memory address

    return imageData;
}



ImageData ImageLoader::loadBMP(const std::string& filepath)
{
    return ImageData();
}
