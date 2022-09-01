#include "avatar-font.h"
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

fs::path fontPath;

AvatarFont::AvatarFont(fs::path fp) {
    bool dirExists = false;

    // Set path for main font directory
    fontPath = fp;

    if (fontPath.filename().string() == "default") {
        isDefaultFont = true;
    }

    // Check that the directories exist for the different size fonts
    // If they don't, create the directory
    cout << "how many font folder: " << fontSizeFolders->length() << endl;

    for (int fd = 0; fd < 3; fd++){
        cout << fontSizeFolders[fd] << " exists? ";

        dirExists = fs::exists(fontSizeFolders[fd]);

        cout << dirExists << endl;
    }
}

bool AvatarFont::isFontDefaultFont() {
    return isDefaultFont;
}

string AvatarFont::showDirectory() {
    return fontPath.filename().string();
}
