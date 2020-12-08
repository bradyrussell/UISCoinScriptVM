//
// Created by Admin on 12/7/2020.
//

#ifndef UISCOINSCRIPTVM_BYTESUTIL_H
#define UISCOINSCRIPTVM_BYTESUTIL_H


#include <vector>
#include <stdint-gcc.h>

namespace BytesUtil {
    std::vector<int8_t> NumberToBytes(int8_t Number);
    std::vector<int8_t> NumberToBytes(int32_t Number);
    std::vector<int8_t> NumberToBytes(int64_t Number);
    std::vector<int8_t> NumberToBytes(float Number);

    int8_t BytesToInt8(const std::vector<int8_t>& Bytes);
    int32_t BytesToInt32(const std::vector<int8_t>& Bytes);
    int64_t BytesToInt64(const std::vector<int8_t>& Bytes);
    float BytesToFloat(const std::vector<int8_t>& Bytes);

    void PrintBytes(std::vector<int8_t> Bytes);
    void PrintBytesHex(std::vector<int8_t> Bytes);


}


#endif //UISCOINSCRIPTVM_BYTESUTIL_H
