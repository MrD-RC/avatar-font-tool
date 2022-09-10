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
        AvatarFont(fs::path);
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