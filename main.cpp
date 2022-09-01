#include <iostream>
#include <vector>
#include <filesystem>
#include "avatar-font.h"

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
    
    // get font directory or set the default to current
    if (argsCount > 1) {
        string tmpPath = args[1];

        if (tmpPath[tmpPath.length() - 1] != '/') {
            tmpPath = tmpPath + "/";
        }

        fs::current_path(tmpPath);
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

        // Output dirs, for testing
        for (auto &dirPath : fontDirectories) {
            // Create objects for fonts
            AvatarFont af(dirPath);
            
            if (af.isFontDefaultFont()) {
                // Add default font object to start of vector
                fonts.insert(fonts.begin(), af);
                haveDefaultFont = true;
            } else {
                // Add font object to vector 
                fonts.push_back(af);
            }

            cout << "Font directory " << af.showDirectory() << " | Have default font? " << haveDefaultFont << endl;
        }

        if (!haveDefaultFont) {
            exitCode = 1;
            cout << "Default font not found! Check that \"default\" is in " << quoted(path.make_preferred().string()) << endl;
        } else {
            // Build fonts from objects
        }
    } else {
        exitCode = 1;
        cout << "No font path! " << path;
    }
    
    return exitCode;
}