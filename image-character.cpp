#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

ImageCharacter::ImageCharacter(const fs::path &imageFile) {
    if (!readImage(imageFile)) {
        cout << "Failed to load image " << imageFile.filename() << endl;
    }
}

ImageCharacter::ImageCharacter(int w, int h) : w(w), h(h) {
    channels = 4;
    size = w * H * channels;
    data = new uint8_t[size];
}

ImageCharacter::ImageCharacter(const ImageCharacter &fromCharacter) {
    
}

ImageCharacter::~ImageCharacter() {
    stbi_image_free(data);
}

bool ImageCharacter::readImage(const fs::path &imageFile) {
    data = stbi_load(imageFile.string(), &w, &h, &channels, 4);
    if (data != NULL) {
        size = width * height * channels;
    } else {
        size = 0;
    }

    return data != NULL;
}

uint8_t* ImageCharacter::getImageData() {

}