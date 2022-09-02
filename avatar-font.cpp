#include "avatar-font.h"
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

fs::path fontPath;

AvatarFont::AvatarFont(fs::path fp) {
    bool dirExists = false;
    fs::file_status s;

    // Set path for main font directory
    fontPath = fp;

    if (fontPath.filename().string() == "default") {
        isDefaultFont = true;
    }

    // Check that the directories exist for the different size fonts
    // If they don't, create the directory
    fontPath36x54 = fontPath.string() + fontPath.root_directory().string() + "36x54";
    fontPath24x36 = fontPath.string() + fontPath.root_directory().string() + "24x36";
    fontPath12x18 = fontPath.string() + fontPath.root_directory().string() + "12x18";

    // 36 x 54 pixels
    s = fs::file_status{};
    dirExists = (fs::status_known(s) ? fs::exists(s) : fs::exists(fontPath36x54));
    if (!dirExists) {
        fs::create_directory(fontPath36x54);
    }

    // 24 x 36 pixels
    s = fs::file_status{};
    dirExists = (fs::status_known(s) ? fs::exists(s) : fs::exists(fontPath24x36));
    if (!dirExists) {
        fs::create_directory(fontPath24x36);
    }

    // 12 x 18 pixels
    s = fs::file_status{};
    dirExists = (fs::status_known(s) ? fs::exists(s) : fs::exists(fontPath12x18));
    if (!dirExists) {
        fs::create_directory(fontPath12x18);
    }  
}

/**
 * @brief 
 * 
 * @param defaultFont   If default font is null, this is the default. Missing characters are 
 *                      filled with empty characters. If this is not default, missing 
 *                      characters are filled from default.
 * @return true if character retrieval successful
 * @return false if there was a problem
 */
bool AvatarFont::generateCharacters(AvatarFont defaultFont) {
    return true;
}

bool AvatarFont::isFontDefaultFont() {
    return isDefaultFont;
}

string AvatarFont::showDirectory() {
    return fontPath.filename().string();
}
