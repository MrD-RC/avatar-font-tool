#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

struct ImageCharacter {
    uint8_t*    data = NULL;
    size_t      size = 0;
    int         w;
    int         h;
    int         channels = 4;

    ImageCharacter(const fs::path &imageFile);
    ImageCharacter(int w, int h) : w(w), h(h);
    ImageCharacter(const ImageCharacter &fromCharacter);
    ~ImageCharacter();

    bool readImage(const fs::path &imageFile);
    uint8_t* getImageData();
};