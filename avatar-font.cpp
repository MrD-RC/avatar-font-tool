#include "avatar-font.h"
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

fs::path fontPath;

AvatarFont::AvatarFont(fs::path fp) {
    // Set path for main font directory
    fontPath = fp;

    // Check that the directories exist for the different size fonts
    for (int fd = 0; fd < (int)AvatarFont::fontSizeFolders->length(); fd++){
        cout << AvatarFont::fontSizeFolders[fd] << " exists? ";


    }
}

string AvatarFont::showDirectory() {
    return fontPath.filename().string();
}
