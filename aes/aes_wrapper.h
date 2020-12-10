//
// Created by Admin on 12/9/2020.
//

#ifndef UISCOINSCRIPTVM_AES_WRAPPER_H
#define UISCOINSCRIPTVM_AES_WRAPPER_H


#include <stdint-gcc.h>
#include <vector>
#include "../crypto/sha512.h"

namespace aes{
    std::vector<int8_t> encrypt(const std::vector<int8_t>& data, const std::vector<int8_t>& key);
    std::vector<int8_t> decrypt(const std::vector<int8_t>& data, const std::vector<int8_t>& key);


    inline std::vector<unsigned char> key_from_hash(const std::vector<int8_t>& key) {
        std::vector<int8_t> vector2 = sha512(key);
        vector2.resize(16);
        return std::vector<unsigned char> (vector2.begin(),vector2.end());
    }
}

#endif //UISCOINSCRIPTVM_AES_WRAPPER_H
