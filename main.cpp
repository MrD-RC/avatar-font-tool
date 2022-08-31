#include <iostream>
#include <vector>
#include <filesystem>

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
    int     exitCode = 0;
    string  path = "./";
    
    // get font directory or set the default to current
    if (argsCount > 1) {
        path = args[1];

        if (path[path.length() - 1] != '/') {
            path = path + "/";
        }
    }

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
            cout << dirPath << endl;
        }
    } else {
        exitCode = 1;
        cout << "No font path! " << path;
    }
    
    return exitCode;
}