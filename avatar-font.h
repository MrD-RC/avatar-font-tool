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
        bool    generateCharacters(AvatarFont defaultFont);
    private:
        map<int, string>    characters12X18;
        map<int, string>    characters24X36;
        map<int, string>    characters36X54;
        bool                isDefaultFont = false;
        fs::path            fontPath;
        fs::path            fontPath36x54;
        fs::path            fontPath24x36;
        fs::path            fontPath12x18;
};