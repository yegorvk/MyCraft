//
// Created by egorv on 4/16/2023.
//

#ifndef SHITCRAFT_ASSETREADER_H
#define SHITCRAFT_ASSETREADER_H

#include <string>
#include <vector>

class AssetReader {
public:
    virtual std::string getText(const char *path) = 0;

    virtual std::vector<unsigned char> getBytes(const char *path) = 0;
};


#endif //SHITCRAFT_ASSETREADER_H
