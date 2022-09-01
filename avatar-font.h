#pragma once
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class AvatarFont {
    public:
        AvatarFont(fs::path);
        string showDirectory();
    private:
        fs::path fontPath;
        string fontSizeFolders[3] = {"36x54", "24x36", "12x18"};
};