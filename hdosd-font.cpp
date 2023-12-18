/**
 * This file is part of HD OSD Font Tool.
 * Written by Darren Lines (Mr. D RC)
 *
 * HD OSD Font Tool is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version. Please keep this header in the files.
 *
 * HD OSD Font Tool is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HD OSD Font Tool.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "hdosd-font.h"
#include "image-character.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

fs::path fontPath;

HDOSDFont::HDOSDFont() {
    // Blank constructor for null object.
}

HDOSDFont::~HDOSDFont() {
}

HDOSDFont::HDOSDFont(fs::path fp, bool isVerbose = false) {
    verbose = isVerbose;
    bool dirExists = false;
    isObjNull = false;
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

void HDOSDFont::seedMaps() {
    // Generate remaining empty characters
    for (int i = 0; i <= 512; i++) {
        ImageCharacter newIC = ImageCharacter();
        characters12X18[i] = newIC;
    }

    for (int i = 0; i <= 512; i++) {
        ImageCharacter newIC = ImageCharacter();
        characters24X36[i] = newIC;
    }

    for (int i = 0; i <= 512; i++) {
        ImageCharacter newIC = ImageCharacter();
        characters36X54[i] = newIC;
    }

    // Set character 0 to blank
    characters12X18[0].setWH(12, 18);
    characters24X36[0].setWH(24, 36);
    characters36X54[0].setWH(36, 54);
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
bool HDOSDFont::generateCharacters(HDOSDFont& defaultFont) {
    if (verbose) {
        cout << "Generating characters for font " << showDirectory() << ". Default Font is " << flush;
        if (defaultFont.isNull()) {
            cout << "NULL" << endl;
        } else {
            cout << defaultFont.showDirectory() << endl;
        }
    }

    seedMaps();

    // Work through the directories and store the images.
    capturePNGCharacters(defaultFont, characters36X54, fontPath36x54, fontPath24x36, fontPath12x18, 36, 54, 24, 36, 12, 18);

    capturePNGCharacters(defaultFont, characters24X36, fontPath24x36, fontPath36x54, fontPath12x18, 24, 36, 36, 54, 12, 18);

    capturePNGCharacters(defaultFont, characters12X18, fontPath12x18, fontPath24x36, fontPath36x54, 12, 18, 24, 36, 36, 54);

    return true;
}

bool HDOSDFont::generateFontImageFiles(fs::path exportPath, string fontBaseName, uint8_t exportFontSystem, bool renderColsx256FontImage) {
    bool aOK = true;

    if (exportFontSystem == OUTPUT_FONT_AVATAR) {
        if (aOK && !generateAvatarPNGFile(exportPath, characters12X18, 12, 18, fontBaseName, renderColsx256FontImage)) {
            cout << "There was an issue generating the Avatar 12x18 " << showDirectory() << " file." << endl;
            aOK = false;
        }

        if (aOK && !generateAvatarPNGFile(exportPath, characters24X36, 24, 36, fontBaseName, renderColsx256FontImage)) {
            cout << "There was an issue generating the Avatar 24x36 " << showDirectory() << " file." << endl;
            aOK = false;
        }

        if (aOK && !generateAvatarPNGFile(exportPath, characters36X54, 36, 54, fontBaseName, renderColsx256FontImage)) {
            cout << "There was an issue generating the Avatar 36x54 " << showDirectory() << " file." << endl;
            aOK = false;
        }
    } else if (exportFontSystem == OUTPUT_FONT_HDZERO) {
        if (aOK && !generateHDZeroBMPFile(exportPath, characters24X36, 24, 36, fontBaseName, renderColsx256FontImage)) {
            cout << "There was an issue generating the HDZero 24x36 " << showDirectory() << " file." << endl;
            aOK = false;
        }

        if (aOK && !generateHDZeroBMPFile(exportPath, characters36X54, 36, 54, fontBaseName, renderColsx256FontImage)) {
            cout << "There was an issue generating the HDZero 36x54 " << showDirectory() << " file." << endl;
            aOK = false;
        }
    }

    return aOK;
}

bool HDOSDFont::generateAvatarPNGFile(fs::path &path, ImageMap& characters, uint8_t charWidth, uint8_t charHeight, string &fontBaseName, bool renderColsx256FontImage) {
    // create the object for the Avatar OSD image
    uint8_t charCols = 1;
    uint16_t charRows = maxCharacters;

    if (renderColsx256FontImage) {
        charCols = (int) maxCharacters / 256;
        charRows = (int) maxCharacters / charCols;
    }

    if (verbose) {
        cout << "Generating a " << to_string(charWidth) << "x" << to_string(charHeight) << " Avatar font with " << to_string(charCols) << " columns and " << to_string(charRows) << " rows." << endl;
    }

    ImageCharacter avatarOutput = ImageCharacter((charWidth * charCols), (charHeight * charRows));   
    
    // Transfer all the individual images to the Avatar image
    uint32_t outImgPtr = 0;
    uint8_t  curCol = 0;
    uint32_t charStart = 0;
    uint8_t  charInd = 0;

    for (auto const&[key, character] : characters) {
        if (key < maxCharacters) {
            charStart = 0;

            if ((outImgPtr > 0) && (charCols > 1) && ((key % charRows) == 0)) {
                curCol++;
                outImgPtr = (charWidth * curCol) * 4;
            }

            for (charInd = 0; charInd < charHeight; charInd++) {
                memcpy(&avatarOutput.data[outImgPtr], character.data + charStart, (charWidth * 4));
                charStart+= (charWidth * 4);
                outImgPtr+= (charWidth * 4) * charCols;
            }
        }
    }

    // Save the file
    return avatarOutput.writeAvatarImage(path.string() + path.root_directory().string() + fontBaseName + showDirectory() + "_" + to_string(charWidth) + ".png");
}

bool HDOSDFont::generateHDZeroBMPFile(fs::path &path, ImageMap& characters, uint8_t charWidth, uint8_t charHeight, string &fontBaseName, bool renderColsx256FontImage) {
    // create the object for the HS OSD image
    bool    verticalFontSet = false;
    uint8_t charCols = 16;
    uint16_t charRows = maxCharacters / 16;

    if (renderColsx256FontImage) {
        charCols = (int) maxCharacters / 256;
        charRows = (int) maxCharacters / charCols;
        verticalFontSet = true;
    }

    if (verbose) {
        cout << "Generating a " << to_string(charWidth) << "x" << to_string(charHeight) << " HDZero font with " << to_string(charCols) << " columns and " << to_string(charRows) << " rows." << endl;
    }

    ImageCharacter hdzeroOutput = ImageCharacter((charWidth * charCols), (charHeight * charRows));
    hdzeroOutput.fillImage(127, 127, 127, 255);
    
    // Transfer all the individual images to the Avatar image
    uint32_t outImgPtr = 0;
    uint8_t  curCol = 0;
    uint8_t  curRow = 0;
    uint32_t charStart = 0;
    uint8_t  charInd = 0;

    for (auto const&[key, character] : characters) {
        if (key < maxCharacters) {
            charStart = 0;

            if (verticalFontSet) {
                if (outImgPtr > 0 && (key % charRows) == 0) {
                    curCol++;
                    outImgPtr = (charWidth * curCol) * 4;
                }
            } else {
                if (outImgPtr > 0) {
                    curCol++;
                }

                if (curCol == charCols) {
                    curRow++;
                    curCol = 0;
                }

                outImgPtr = ((charWidth * 4) * curCol) + (((charWidth * 4) * charCols) * (charHeight * curRow));
            }

            for (charInd = 0; charInd < charHeight; charInd++) {
                for (uint8_t pixel = 0; pixel < charWidth; pixel++) {
                    if ((character.data[(charStart + (pixel * 4) + 3)] > 10)) {
                        memcpy(&hdzeroOutput.data[outImgPtr+(pixel * 4)], character.data + (charStart + (pixel*4)), 4);
                    }
                }
                charStart+= (charWidth * 4);
                outImgPtr+= (charWidth * 4) * charCols;
            }
        }
    }

    // Save the file
    return hdzeroOutput.writeHDZeroImage(path.string() + path.root_directory().string() + fontBaseName + showDirectory() + "_" + to_string(charWidth) + ".bmp");
}

bool HDOSDFont::isFontDefaultFont() {
    return isDefaultFont;
}

string HDOSDFont::showDirectory() {
    return fontPath.filename().string();
}

int HDOSDFont::getMaxCharacters() {
    return maxCharacters;
}

ImageMap &HDOSDFont::getFontMap(string fontSize) {
    if (fontSize == "36x54") {
        return characters36X54;
    } else if (fontSize == "24x36") {
        return characters24X36;
    } 
    
    // else (fontSize == "12x18")
    return characters12X18;
}

int HDOSDFont::calculateLastCharacter(HDOSDFont& defaultFont) {
    return (isDefaultFont) ? maxCharacters : defaultFont.getMaxCharacters();
}

void HDOSDFont::capturePNGCharacters(HDOSDFont& defaultFont, ImageMap& characterMap, fs::path charactersPath, fs::path alt1, fs::path alt2, uint8_t thisWidth, uint8_t thisHeight, uint8_t alt1Width, uint8_t alt1Height, uint8_t alt2Width, uint8_t alt2Height) {
    int characterIndex = 0;
    int lastCharacter = calculateLastCharacter(defaultFont);
    int capturedCharacters = 0;

    if (verbose) {
        cout << "Capturing characters for " << quoted(charactersPath.string()) << flush;
    }

    for (const auto &file : fs::directory_iterator{charactersPath}) {
        string extension = file.path().extension().string();
        transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        if (extension == ".png") {
            string imagePath = file.path().parent_path().string() + file.path().root_directory().string();
            string filename = file.path().filename().stem().string();

            // Process the PNG
            if (filename.find('-') != string::npos) {
                // This is multiple characters in one PNG
                // each character is separated by a '-'
                string tmpFilename = filename;
                std::replace(tmpFilename.begin(), tmpFilename.end(), '-', ' ');
                istringstream fNameStream(tmpFilename);
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
                // As we are dealing with multiple images per character, we need to do some image manipulation here.
                // 1. Load the entire image
                ImageCharacter fullCharacterImg = ImageCharacter(imagePath + filename + ".png");
                uint16_t ptrX = 0;
                uint16_t ptrY = 0;

                // 2. Iterate through the number of individual characters and copy the image data from the full image to the partial character image.
                for (const auto &currentFilename : filenames) {
                    workingCharacter = stoi(currentFilename);

                    characterMap[workingCharacter].setWH(thisWidth, thisHeight);
                    uint8_t y = 0;
                    uint8_t x = 0;

                    for (y = 0; y < thisHeight; ++y) {
                        if (y + ptrY >= fullCharacterImg.h) {
                            cout << filename << ": height out of range" << endl;
                            break;
                        }
                        for (x = 0; x < thisWidth; ++x) {
                             if (x + ptrX >= fullCharacterImg.w) {
                                cout << filename << ": width out of range" << endl;
                                break;
                            }

                            memcpy( &characterMap[workingCharacter].data[(x + y * thisWidth) * characterMap[workingCharacter].channels], 
                                    &fullCharacterImg.data[(x + ptrX + (y + ptrY) * fullCharacterImg.w) * fullCharacterImg.channels], 
                                    characterMap[workingCharacter].channels);
                        }
                    }

                    if ((x + ptrX) >= fullCharacterImg.w) {
                        ptrX = 0;
                        ptrY+= y;
                    } else {
                        ptrX+= x;
                    }

                    if (characterIndex == workingCharacter) {
                        characterIndex++;
                    }
                }

                // Update max and last characters
                maxCharacters = (floor((float)workingCharacter / 256) + 1) * 256;
                lastCharacter = calculateLastCharacter(defaultFont);

                capturedCharacters++;
            } else if (filename.find('_') != string::npos) {
                // This is multiple characters in one PNG
                // The characters must be sequential. The first character and last character indexes are separated by an '_'
                string tmpFilename = filename;
                std::replace(tmpFilename.begin(), tmpFilename.end(), '_', ' ');
                istringstream fNameStream(tmpFilename);
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
                // As we are dealing with multiple images per character, we need to do some image manipulation here.
                // 1. Load the entire image
                ImageCharacter fullCharacterImg = ImageCharacter(imagePath + filename + ".png");
                uint16_t ptrX = 0;
                uint16_t ptrY = 0;
                
                // 2. Iterate through the number of individual characters and copy the image data from the full image to the partial character image.
                for (int fc = stoi(filenames[0]); fc <= stoi(filenames[1]); fc++) {
                    workingCharacter = fc;

                    characterMap[workingCharacter].setWH(thisWidth, thisHeight);
                    uint8_t y;
                    uint8_t x;

                    for (y = 0; y < thisHeight; ++y) {
                        if (y + ptrY >= fullCharacterImg.h) {
                            cout << filename << ": height out of range" << endl;
                            break;
                        }
                        for (x = 0; x < thisWidth; ++x) {
                             if (x + ptrX >= fullCharacterImg.w) {
                                cout << filename << ": width out of range" << endl;
                                break;
                            }

                            memcpy( &characterMap[workingCharacter].data[(x + y * thisWidth) * characterMap[workingCharacter].channels], 
                                    &fullCharacterImg.data[(x + ptrX + (y + ptrY) * fullCharacterImg.w) * fullCharacterImg.channels], 
                                    characterMap[workingCharacter].channels);
                        }
                    }

                    if ((x + ptrX) >= fullCharacterImg.w) {
                        ptrX = 0;
                        ptrY+= y;
                    } else {
                        ptrX+= x;
                    }

                    characterIndex++;
                }

                // Update max and last characters
                maxCharacters = (floor((float)workingCharacter / 256) + 1) * 256;
                lastCharacter = calculateLastCharacter(defaultFont);

                capturedCharacters++;
            } else {
                int workingCharacter = stoi(filename);
                
                // If we have missing characters, fill the gaps
                if (characterIndex < workingCharacter) {
                    characterIndex = findMissingCharacters(defaultFont, characterMap, characterIndex, workingCharacter, alt1, alt2, thisWidth, thisHeight, alt1Width, alt1Height, alt2Width, alt2Height);
                }

                // We have a single character, so process it
                characterMap[characterIndex].readImage(imagePath + filename + ".png");

                characterIndex++;

                // Update max and last characters
                maxCharacters = (floor((float)workingCharacter / 256) + 1) * 256;
                lastCharacter = calculateLastCharacter(defaultFont);

                capturedCharacters++;
            }
        }
    }

    maxCharacters = lastCharacter;

    if (verbose) {
        cout << " with a total of " << lastCharacter << " characters. " << to_string(capturedCharacters) << " characters loaded from directory." << endl;
    }

    if (characterIndex < (lastCharacter + 1)) {
        characterIndex = findMissingCharacters(defaultFont, characterMap, characterIndex, (lastCharacter + 1), alt1, alt2, thisWidth, thisHeight, alt1Width, alt1Height, alt2Width, alt2Height);
    }

    return;
}

int HDOSDFont::findMissingCharacters(HDOSDFont& defaultFont, ImageMap& characterMap, int characterIndex, int workingCharacter, fs::path alt1, fs::path alt2, uint8_t thisWidth, uint8_t thisHeight, uint8_t alt1Width, uint8_t alt1Height, uint8_t alt2Width, uint8_t alt2Height) {
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


        // Check that the character has not already been fulfilled by a multiple character image
        if (characterMap[characterIndex].data == NULL) {
            // Check path alt 1
            fs::path tmpPath = alt1.string() + alt1.root_directory().string() + searchFile;

            if (fs::exists(tmpPath)) {
                characterMap[characterIndex].resizeImage(tmpPath.string(), thisWidth, thisHeight);
            } else {
                // Check path alt 2
                tmpPath = alt2.string() + alt2.root_directory().string() + searchFile;

                if (fs::exists(tmpPath)) {
                    characterMap[characterIndex].resizeImage(tmpPath.string(), thisWidth, thisHeight);
                } else {
                    if (!defaultFont.isNull() && !isDefaultFont) {
                        // Use the image from the default font
                        string fontSize = to_string(thisWidth) + "x" + to_string(thisHeight);
                        characterMap[characterIndex].copyImage(defaultFont.getFontMap(fontSize)[characterIndex]);
                    } else {
                        // No character found. Create an empty image
                        characterMap[characterIndex].setWH(thisWidth, thisHeight);
                    }
                }
            }
        }

        characterIndex++;
    }

    return characterIndex;
}