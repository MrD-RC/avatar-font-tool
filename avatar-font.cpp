#include "avatar-font.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <map>
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
    } else {
        getCharacterCount(fontPath36x54);
    }

    // 24 x 36 pixels
    s = fs::file_status{};
    dirExists = (fs::status_known(s) ? fs::exists(s) : fs::exists(fontPath24x36));
    if (!dirExists) {
        fs::create_directory(fontPath24x36);
    } else {
        getCharacterCount(fontPath24x36);
    }

    // 12 x 18 pixels
    s = fs::file_status{};
    dirExists = (fs::status_known(s) ? fs::exists(s) : fs::exists(fontPath12x18));
    if (!dirExists) {
        fs::create_directory(fontPath12x18);
    } else {
        getCharacterCount(fontPath12x18);
    }
}

void AvatarFont::getCharacterCount(fs::path characterPath) {
    if (fontSize != 512) {
        string  filename = "";
        int     characterNumber = 0;
        for (const auto &file : fs::directory_iterator(characterPath.string())) {
            filename = file.path().stem().string();
            characterNumber = stoi(filename);

            if (characterNumber > 256) {
                fontSize = 512;
                return;
            }
        }
    }

    return;
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
    //capturePNGCharacters(defaultFont, characters36X54, fontPath36x54, fontPath24x36, fontPath12x18, 36, 54, 24, 36, 12, 18);

    capturePNGCharacters(defaultFont, characters24X36, fontPath24x36, fontPath36x54, fontPath12x18, 24, 36, 36, 54, 12, 18);

    cout << "MAP:" << endl;
    for (const auto &[key, value] : characters24X36) {
        cout << "| " << setfill('0') << setw(3) << key << " | " << value << " |" << endl;
    }

    //capturePNGCharacters(defaultFont, characters24X36, fontPath12x18, fontPath24x36, fontPath36x54, 12, 18, 24, 36, 36, 54);

    return true;
}

bool AvatarFont::isFontDefaultFont() {
    return isDefaultFont;
}

string AvatarFont::showDirectory() {
    return fontPath.filename().string();
}

int AvatarFont::getFontSize() {
    return fontSize;
}

map<int, string> &AvatarFont::getFontMap(string fontSize) {
    if (fontSize == "36x54") {
        return characters36X54;
    } else if (fontSize == "24x36") {
        return characters24X36;
    } 
    
    // else (fontSize == "12x18")
    return characters12X18;
}


void AvatarFont::capturePNGCharacters(AvatarFont *defaultFont, map<int, string> &characterMap, fs::path charactersPath, fs::path alt1, fs::path alt2, int thisWidth, int thisHeight, int alt1Width, int alt1Height, int alt2Width, int alt2Height) {
    int characterIndex = 1;
    int lastCharacter = (isDefaultFont) ? fontSize : defaultFont->getFontSize();

    cout << "Capturing characters for " << quoted(charactersPath.string()) << " with a total of " << lastCharacter << " characters. " << endl;

    for (const auto &file : fs::directory_iterator{charactersPath}) {
        string extension = file.path().extension().string();
        transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        if (extension == ".png") {
            string filename = file.path().filename().stem().string();

            // Process the PNG
            if (filename.find('-') != string::npos) {
                // This is two characters in one PNG
                std::replace(filename.begin(), filename.end(), '-', ' ');
                istringstream fNameStream(filename);
                string tmp;
                vector<string> filenames;
                while (fNameStream >> tmp) {
                    filenames.push_back(tmp);
                }

                int workingCharacter = stoi(filenames[0]);

                // If we have missing characters, fill the gaps
                if (characterIndex < workingCharacter) {
                    characterIndex = findMissingCharacters(defaultFont, characterMap, characterIndex, workingCharacter, alt1, alt2, thisWidth, thisHeight, alt1Width, alt1Height, alt2Width, alt2Height);
                }

                // Process the characters
                for (const auto &currentFilename : filenames) {
                    characterMap[characterIndex] = "IMAGE " + currentFilename;
                    workingCharacter = stoi(currentFilename);
                    characterIndex++;
                }
            } else if (filename.find('_') != string::npos) {
                // This is multiple characters in one PNG
                std::replace(filename.begin(), filename.end(), '_', ' ');
                istringstream fNameStream(filename);
                string tmp;
                vector<string> filenames;
                while (fNameStream >> tmp) {
                    filenames.push_back(tmp);
                }

                int workingCharacter = stoi(filenames[0]);

                // If we have missing characters, fill the gaps
                if (characterIndex < workingCharacter) {
                    characterIndex = findMissingCharacters(defaultFont, characterMap, characterIndex, workingCharacter, alt1, alt2, thisWidth, thisHeight, alt1Width, alt1Height, alt2Width, alt2Height);
                }

                // Process the characters
                for (int fc = stoi(filenames[0]); fc <= stoi(filenames[1]); fc++) {
                    characterMap[characterIndex] = "IMAGE " + to_string(fc);
                    workingCharacter = fc;
                    characterIndex++;
                }
            } else {
                int workingCharacter = stoi(filename);
                
                // If we have missing characters, fill the gaps
                if (characterIndex < workingCharacter) {
                    characterIndex = findMissingCharacters(defaultFont, characterMap, characterIndex, workingCharacter, alt1, alt2, thisWidth, thisHeight, alt1Width, alt1Height, alt2Width, alt2Height);
                }

                // We have a single character, so process it
                characterMap[characterIndex] = "IMAGE " + filename;

                characterIndex++;
            }
        }
    }

    if (characterIndex < (lastCharacter + 1)) {
        characterIndex = findMissingCharacters(defaultFont, characterMap, characterIndex, (lastCharacter + 1), alt1, alt2, thisWidth, thisHeight, alt1Width, alt1Height, alt2Width, alt2Height);
    }

    return;
}

int AvatarFont::findMissingCharacters(AvatarFont *defaultFont, map<int, string> &characterMap, int characterIndex, int workingCharacter, fs::path alt1, fs::path alt2, int thisWidth, int thisHeight, int alt1Width, int alt1Height, int alt2Width, int alt2Height) {
    // If we have missing characters, fill the gaps
    while (characterIndex < workingCharacter) {
        // We are missing a character, so handle that
        // 1. does it exist in another size folder?
        // 2. does it exist in default?
        // 3. use a blank character

        // Generate the filename of the missing file
        string searchFile = to_string(characterIndex);
        size_t searchFileChars = 3;
        int precision = searchFileChars - min(searchFileChars, searchFile.size());
        searchFile.insert(0, precision, '0');
        searchFile += ".png";

        // Check path alt 1
        fs::path tmpPath = alt1.string() + alt1.root_directory().string() + searchFile;
        if (fs::exists(tmpPath)) {
            characterMap[characterIndex] = "ALT1 IMAGE" + searchFile;
        } else {
            // Check path alt 1
            tmpPath = alt2.string() + alt2.root_directory().string() + searchFile;
            if (fs::exists(tmpPath)) {
                characterMap[characterIndex] = "ALT2 IMAGE" + searchFile;
            } else {
                if (!isDefaultFont) {
                    string fontSize = to_string(thisWidth) + "x" + to_string(thisHeight);

                    characterMap[characterIndex] = "DEFAULT " + (defaultFont->getFontMap(fontSize))[characterIndex];
                } else {
                    characterMap[characterIndex] = "BLANK";
                }
            }
        }

        characterIndex++;
    }

    return characterIndex;
}