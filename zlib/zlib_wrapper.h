//
// Created by Admin on 12/8/2020.
//

#ifndef UISCOINSCRIPTVM_ZLIB_WRAPPER_H
#define UISCOINSCRIPTVM_ZLIB_WRAPPER_H


#include <vector>
#include <stdint-gcc.h>

namespace zlib {
    std::vector<int8_t> zip(const std::vector<int8_t>& input);
    std::vector<int8_t> unzip(const std::vector<int8_t>& input);
};


#endif //UISCOINSCRIPTVM_ZLIB_WRAPPER_H
