#pragma once
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class ImageCharacter {
    public:
        ImageCharacter();
        ~ImageCharacter();
        ImageCharacter(const string imageFile);
        ImageCharacter(int newW, int newH);
        ImageCharacter(const ImageCharacter &fromCharacter);

        void copyImage(const ImageCharacter &fromCharacter);
        bool readImage(const string imageFile);
        void resizeImage(const string imageFile, int newW, int newH);
        void setWH(int newW, int newH);
        bool writeImage(const string fileName);

        int         w;
        int         h;
        int         channels = 4;
        uint8_t*    data = NULL;
        size_t      size = 0;
    private:
};