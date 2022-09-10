#include <iostream>
#include <vector>
#include <filesystem>
#include "avatar-font.h"

#include "image-character.h" // REMOVE AFTER TEST

using namespace std;
namespace fs = std::filesystem;

/**
 * Folder structure
 * 
 * fonts
 *    |-- font
 *          |-- 12 x 18
 *          |-- 24 x 36
 *          |-- 36 x 54
 */
int main(int argsCount, char *args[]) {
    int                 exitCode = 0;
    vector<AvatarFont>  fonts;
    bool                haveDefaultFont = false;
    fs::path            path;
    string              fontBaseName = "";
    
    // get font directory or set the default to current
    if (argsCount > 1) {
        string tmpPath = args[1];

        if (tmpPath == "?") {
            cout << "Avatar Font Tool by Mr. D (www.mrd-rc.com)" << endl
                 << "Command line usage:" << endl
                 << "no arguments - default use: font root in current directory and no filename prefix" << endl
                 << "? - this help screen" << endl
                 << "argument 1 - font root location" << endl
                 << "argument 2 - filename prefix" << endl;

            exit(exitCode);
        }

        if (tmpPath[tmpPath.length() - 1] != '/') {
            tmpPath = tmpPath + "/";
        }

        fs::current_path(tmpPath);

        if (argsCount > 2) {
            fontBaseName = args[2];

            if (fontBaseName.length() > 0) {
                fontBaseName+= "_";
            }
        }
    }

    path = fs::current_path();

    fs::file_status s = fs::file_status{};
    const fs::path& p = path;
    if (fs::status_known(s) ? fs::exists(s) : fs::exists(p)) {
        // The base fonts directory exists.
 
        // find sub directory for font files
        vector<fs::path> fontDirectories;
        for (const auto &entry : fs::directory_iterator(p)) {
            if (fs::is_directory(entry)) {
                fontDirectories.push_back(entry);
            }
        }

        // Create the font objects based on the subdirectories of the current directory. These should all be fonts
        // First loop through the subdirectories and create the AvatarFont objects
        for (auto &dirPath : fontDirectories) {
            // Create objects for fonts
            AvatarFont af(dirPath);
            
            if (af.isFontDefaultFont()) {
                // Add default font object to start of vector
                fonts.insert(fonts.begin(), af);
                // State that we have a default font
                haveDefaultFont = true;
            } else {
                // Add font object to vector 
                fonts.push_back(af);
            }
        }

        // If we don't have a default font. We should give an error message and exit the program
        if (!haveDefaultFont) {
            exitCode = 1;
            cout << "Default font not found! Check that \"default\" is in " << quoted(path.make_preferred().string()) << endl;
            exit(exitCode);
        }

        // Now that we have all our font objects, we should proceed to process. The default font first, then the rest. Missing characters can be filled from default.
        for (auto &fontSet : fonts) {
            cout << "Have font: " << fontSet.showDirectory() << ". Is it the default font? " << ((fontSet.isFontDefaultFont()) ? "Yes" : "No") << endl;

            // Generate the image data from the PNG character files.
            if (fontSet.isFontDefaultFont()) {
                AvatarFont null = AvatarFont();
                fontSet.generateCharacters(null);
            } else {
                fontSet.generateCharacters(fonts[0]);
            }

            // Output the font files. The file name format is fontBaseName_font folder_font width. 
            // For example with arg fontBaseName set to INAV. INAV_default_24.png
            // ... or with no fontBaseName set. default_24.png
            fontSet.generateAvatarPNGFiles(path, fontBaseName);

            cout << "Font generation successful." << endl;
        }
    } else {
        exitCode = 1;
        cout << "No font path! " << path;
    }
    
    return exitCode;
}