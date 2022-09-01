#pragma once
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class AvatarFont {
    public:
        AvatarFont(fs::path);
        string  showDirectory();
        bool    isFontDefaultFont();
    private:
        bool            isDefaultFont = false;
        fs::path        fontPath;
        const string    fontSizeFolders[3] = {"36x54", "24x36", "12x18"};
};