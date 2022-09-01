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
        const string        fontSizeFolders[3] = {"36x54", "24x36", "12x18"};
};