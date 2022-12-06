/**
 * This file is part of Avatar Font Tool.
 * Written by Darren Lines (Mr. D RC)
 *
 * Avatar Font Tool is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version. Please keep this header in the files.
 *
 * Avatar Font Tool is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Avatar Font Tool.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    string              tmpPath = "";
    fs::path            path;
    string              fontBaseName = "";
    const char          newl = '\n';
    bool                verbose = false;
    bool                renderColsx256FontImage = false;
    
    // get font directory or set the default to current
    if (argsCount > 1) {
        string arg = args[1];
        if (arg == "?") {
            cout << "Avatar Font Tool by Mr. D (www.mrd-rc.com)" << newl
                 << "Command line usage:" << newl
                 << "no arguments - default use: font root in current directory and no filename prefix" << newl
                 << "-- Options --" << newl
                 << "?      | This help screen" << newl
                 << "-p %p  | Path to the font root" << newl
                 << "-n %n  | Filename prefix" << newl
                 << "-xcols | Output a multi-column x 256 high character font image" << newl
                 << "-v     | Verbose mode (show more detailed messages)" << endl;

            exit(exitCode);
        } else if (arg == "-v") {
            verbose = true;
        }

        if (argsCount > 2) {
            int curArg = 1;

            while (argsCount > curArg) {
                string option = args[curArg++];

                if (option == "-v") {
                    verbose = true;
                } else if (option == "-xcols") {
                    renderColsx256FontImage = true;
                } else if (argsCount > curArg) {
                    if (option == "-p") {
                        string value  = args[curArg++];
                        tmpPath = value;

                        if (tmpPath[tmpPath.length() - 1] != '/') {
                            tmpPath = tmpPath + "/";
                        }

                        fs::current_path(tmpPath);
                    }

                    if (option == "-n") {
                        string value  = args[curArg++];
                        fontBaseName = value;

                        if (fontBaseName.length() > 0) {
                            fontBaseName+= "_";
                        }
                    }
                }
            }
        }
    }

    path = fs::current_path();

    fs::file_status s = fs::file_status{};
    const fs::path& p = path;

    if (verbose) {
        cout << "Staring program for font " << fontBaseName << " in directory " << quoted(path.string()) << endl;
    }

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
            AvatarFont af(dirPath, verbose);
            
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
            if (verbose) {
                cout << "Have font: " << fontSet.showDirectory() << ". Is it the default font? " << ((fontSet.isFontDefaultFont()) ? "Yes" : "No") << endl;
            }

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
            fontSet.generateAvatarPNGFiles(path, fontBaseName, renderColsx256FontImage);

            cout << "Font generation successful." << endl;
        }
    } else {
        exitCode = 1;
        cout << "No font path! " << path;
    }
    
    return exitCode;
}