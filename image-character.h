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
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class ImageCharacter {
    public:
        ImageCharacter();
        ~ImageCharacter();
        ImageCharacter(const string imageFile);
        ImageCharacter(int newW, int newH);
        ImageCharacter(const ImageCharacter &fromCharacter);

        void copyImage(const ImageCharacter &fromCharacter);
        bool readImage(const string imageFile);
        void fillImage(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void resizeImage(const string imageFile, int newW, int newH);
        void setWH(int newW, int newH);
        bool writeAvatarImage(const string fileName);
        bool writeHDZeroImage(const string fileName);

        int         w;
        int         h;
        int         channels = 4;
        uint8_t*    data = NULL;
        size_t      size = 0;
    private:
};