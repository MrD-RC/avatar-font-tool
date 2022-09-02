#include "avatar-font.h"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <vector>
//#include "libraries/CImg.h"

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
bool AvatarFont::generateCharacters(AvatarFont *defaultFont) {
    cout << "Generating characters for font " << showDirectory() << ". Default Font is ";
    if (defaultFont == NULL) {
        cout << "NULL" << endl;
    } else {
        cout << defaultFont->showDirectory() << endl;
    }

    // Work through the directories and store the images.
    capturePNGCharacters(defaultFont, fontPath36x54, fontPath24x36, fontPath12x18, 36, 54, 24, 36, 12, 18);

    capturePNGCharacters(defaultFont, fontPath24x36, fontPath36x54, fontPath12x18, 24, 36, 36, 54, 12, 18);

    capturePNGCharacters(defaultFont, fontPath12x18, fontPath24x36, fontPath36x54, 12, 18, 24, 36, 36, 54);

    return true;
}

bool AvatarFont::isFontDefaultFont() {
    return isDefaultFont;
}

string AvatarFont::showDirectory() {
    return fontPath.filename().string();
}


void AvatarFont::capturePNGCharacters(AvatarFont *defaultFont, fs::path charactersPath, fs::path alt1, fs::path alt2, int thisWidth, int thisHeight, int alt1Width, int alt1Height, int alt2Width, int alt2Height) {
    int characterIndex = 1;

    for (const auto &file : fs::directory_iterator{charactersPath}) {
        string extension = file.path().extension().string();
        transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        if (extension == ".png") {
            string filename = file.path().filename().stem().string();
            cout << "File " << quoted(filename);
            cout << " | Image " << setfill('0') << setw(3) << characterIndex;
            // Process the PNG
            if (filename.find('-') != string::npos) {
                // This is two characters in one PNG
                cout << " is two images";
                
                std::replace(filename.begin(), filename.end(), '-', ' ');
                istringstream fNameStream(filename);
                string tmp;
                vector<string> filenames;
                while (fNameStream >> tmp) {
                    filenames.push_back(tmp);
                }

                int workingCharacter = stoi(filenames[0]);

                // If we have missing characters, fill the gaps
                while (characterIndex < workingCharacter) {
                    // We are missing a character, so handle that
                    // 1. does it exist in another size folder?
                    // 2. does it exist in default?
                    // 3. use a blank character

                    characterIndex++;
                }

                // Process the characters
                //cout << filenames[1];
                characterIndex = stoi(filenames[1]);

                characterIndex++;
            } else if (filename.find('_') != string::npos) {
                // This is multiple characters in one PNG
                cout << " is multiple images";

                std::replace(filename.begin(), filename.end(), '_', ' ');
                istringstream fNameStream(filename);
                string tmp;
                vector<string> filenames;
                while (fNameStream >> tmp) {
                    filenames.push_back(tmp);
                }

                int workingCharacter = stoi(filenames[0]);

                // If we have missing characters, fill the gaps
                while (characterIndex < workingCharacter) {
                    // We are missing a character, so handle that
                    // 1. does it exist in another size folder?
                    // 2. does it exist in default?
                    // 3. use a blank character

                    characterIndex++;
                }

                // Process the characters 
                characterIndex = stoi(filenames[1]);

                characterIndex++;
            } else {
                int workingCharacter = stoi(filename);
                
                // If we have missing characters, fill the gaps
                while (characterIndex < workingCharacter) {
                    // We are missing a character, so handle that
                    // 1. does it exist in another size folder?
                    // 2. does it exist in default?
                    // 3. use a blank character

                    characterIndex++;
                }

                // We have a single character, so process it
                cout << " is single image";
                characterIndex++;
            }
        }

        cout << endl;
    }
}