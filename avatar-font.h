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

#pragma once
#include "image-character.h"
#include <filesystem>
#include <map>

using namespace std;
namespace fs = std::filesystem;

typedef map<int, ImageCharacter> ImageMap;

class AvatarFont {
    public:
        AvatarFont();
        ~AvatarFont();
        AvatarFont(fs::path, bool);
        string                  showDirectory();
        int                     getMaxCharacters();
        bool                    isFontDefaultFont();
        bool                    isNull() {return isObjNull;}
        bool                    generateCharacters(AvatarFont& defaultFont);
        bool                    generateAvatarPNGFiles(fs::path exportPath, string fontBaseName);
        ImageMap                &getFontMap(string fontSize);
    private:
        ImageMap                characters12X18;
        ImageMap                characters24X36;
        ImageMap                characters36X54;
        bool                    verbose         = false;
        bool                    isObjNull       = true;
        bool                    isDefaultFont   = false;
        int                     maxCharacters   = 256;
        fs::path                fontPath;
        fs::path                fontPath36x54;
        fs::path                fontPath24x36;
        fs::path                fontPath12x18;

        int                     calculateLastCharacter(AvatarFont& defaultFont);
        void                    capturePNGCharacters(AvatarFont& defaultFont, map<int, ImageCharacter>& characterMap, fs::path charactersPath, fs::path alt1, fs::path alt2, uint8_t thisWidth, uint8_t thisHeight, uint8_t alt1Width, uint8_t alt1Height, uint8_t alt2Width, uint8_t alt2Height);
        int                     findMissingCharacters(AvatarFont& defaultFont, map<int, ImageCharacter>& characterMap, int characterIndex, int workingCharacter, fs::path alt1, fs::path alt2, uint8_t thisWidth, uint8_t thisHeight, uint8_t alt1Width, uint8_t alt1Height, uint8_t alt2Width, uint8_t alt2Height);
        bool                    generatePNGFile(fs::path &path, map<int, ImageCharacter>& characterMap, uint8_t charWidth, uint8_t charHeight, string &fontBaseName);
        void                    seedMaps();
};