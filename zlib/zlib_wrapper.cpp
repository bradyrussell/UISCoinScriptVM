//
// Created by Admin on 12/8/2020.
//

#include <zconf.h>
#include <zlib.h>
#include <stdexcept>
#include <iostream>
#include "zlib_wrapper.h"

std::vector<int8_t> zlib::zip(const std::vector<int8_t> &input) {
    unsigned char CompressedBytes[compressBound(input.size())];
    uLong CompressedBufferSize = sizeof(CompressedBytes);

    int Error = compress(CompressedBytes, &CompressedBufferSize, (const Bytef*)(&input.at(0)), input.size());

    if(Error != Z_OK) throw std::runtime_error("Zlib compress failed! Error: " + std::to_string(Error));

    return std::vector<int8_t>(CompressedBytes, CompressedBytes + CompressedBufferSize);
}

std::vector<int8_t> zlib::unzip(const std::vector<int8_t> &input) {
    unsigned char UncompressedBytes[10 * (input.size())]; // hopefully enough
    uLong UncompressedBufferSize = sizeof(UncompressedBytes);

    int Error = uncompress(UncompressedBytes, &UncompressedBufferSize, (const Bytef*)(&input.at(0)), input.size());

    if(Error != Z_OK) throw std::runtime_error("Zlib uncompress failed! Error: " + std::to_string(Error));

    return std::vector<int8_t>(UncompressedBytes, UncompressedBytes + UncompressedBufferSize);
}
