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
    for (int fd = 0; fd < (end(fontSizeFolders) - begin(fontSizeFolders)); fd++){
        s = fs::file_status{};
        const fs::path& p = (fontPath.string() + fontPath.root_directory().string() + fontSizeFolders[fd]);

        dirExists = (fs::status_known(s) ? fs::exists(s) : fs::exists(p));

        if (!dirExists) {
            fs::create_directory(p);
        }
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
