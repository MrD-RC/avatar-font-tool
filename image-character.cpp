#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "image-character.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

ImageCharacter::ImageCharacter() {
    // Need a blank constructor, failing to compile without it.
}

ImageCharacter::~ImageCharacter() {
    stbi_image_free(data);
}

ImageCharacter::ImageCharacter(const string imageFile) {
    if (!readImage(imageFile)) {
        cout << "Failed to load image " << imageFile << endl;
    }
}

ImageCharacter::ImageCharacter(int newW, int newH) {
    setWH(newW, newH);
}

ImageCharacter::ImageCharacter(const ImageCharacter &fromCharacter) {
    copyImage(fromCharacter);
}

void ImageCharacter::copyImage(const ImageCharacter &fromCharacter) {
    w = fromCharacter.w;
    h = fromCharacter.h;
    channels = fromCharacter.channels;
    size = w * h * channels;
    data = new uint8_t[size](0);
    memcpy(data, fromCharacter.data, size);
}

bool ImageCharacter::readImage(const string imageFile) {
    char filename[imageFile.length() + 1];
    strcpy(filename, imageFile.c_str());
    data = stbi_load(filename, &w, &h, &channels, 4);
    
    if (data != NULL) {
        size = w * h * channels;
    } else {
        size = 0;
    }

    return data != NULL;
}


void ImageCharacter::resizeImage(const string imageFile, int newW, int newH) {
    if (!readImage(imageFile)) {
        cout << "Failed to load image " << imageFile << endl;
    }

    uint8_t* resizedData = new uint8_t[(newW * newH * channels)](0);
    stbir_resize_uint8( data, w, h, 0, 
                        resizedData, newW, newH, 0, channels);
    w = newW;
    h = newH;
    size = w * h * channels;
    data = resizedData;
}

void ImageCharacter::setWH(int newW, int newH) {
    w = newW;
    h = newH;
    size = w * h * channels;
    data = new uint8_t[size](0);

    // Convert to a transparent, white image
    for (int i = 0; i < (w * h * channels); i++) {
        if (i % 4 == 3) {
            data[i] = 0;
        } else {
            data[i] = 255;
        }
    }
}

bool ImageCharacter::writeImage(const string fileName) {
    stbi_write_png_compression_level = 10;

    char filename[fileName.length() + 1];
    strcpy(filename, fileName.c_str());

    int success = stbi_write_png(filename, w, h, channels, data, (w * channels));
    
    return success != 0;
}