/**
 * This file is part of HD OSD Font Tool.
 * Written by Darren Lines (Mr. D RC)
 *
 * HD OSD Font Tool is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version. Please keep this header in the files.
 *
 * HD OSD Font Tool is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HD OSD Font Tool.  If not, see <http://www.gnu.org/licenses/>.
 */

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

void ImageCharacter::fillImage(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    for (int i = 0; i < (w * h * channels); i++) {
        data[i++] = r;
        data[i++] = g;
        data[i++] = b;
        data[i] = a;
    }
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

bool ImageCharacter::writeAvatarImage(const string fileName) {
    stbi_write_png_compression_level = 10;

    char filename[fileName.length() + 1];
    strcpy(filename, fileName.c_str());

    int success = stbi_write_png(filename, w, h, channels, data, (w * channels));
    
    return (success != 0);
}

bool ImageCharacter::writeHDZeroImage(const string fileName) {
    char filename[fileName.length() + 1];
    strcpy(filename, fileName.c_str());

    int success = stbi_write_bmp(filename, w, h, channels, data);

    return (success != 0);
}