#pragma once
#include <filesystem>
#include <map>

using namespace std;
namespace fs = std::filesystem;

class AvatarFont {
    public:
        AvatarFont(fs::path);
        string  showDirectory();

        bool    isFontDefaultFont();
        bool    generateCharacters(AvatarFont *defaultFont);
    private:
        map<int, string>    characters12X18;
        map<int, string>    characters24X36;
        map<int, string>    characters36X54;
        bool                isDefaultFont = false;
        int                 fontSize = 255;
        fs::path            fontPath;
        fs::path            fontPath36x54;
        fs::path            fontPath24x36;
        fs::path            fontPath12x18;

        void capturePNGCharacters(AvatarFont *defaultFont, fs::path charactersPath, fs::path alt1, fs::path alt2, int thisWidth, int thisHeight, int alt1Width, int alt1Height, int alt2Width, int alt2Height);
};