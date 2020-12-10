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
    std::vector<int8_t> BooleanToBytes(bool Boolean);

    bool BytesToBoolean(const std::vector<int8_t>& Bytes);
    int8_t BytesToInt8(const std::vector<int8_t>& Bytes);
    int32_t BytesToInt32(const std::vector<int8_t>& Bytes);
    int64_t BytesToInt64(const std::vector<int8_t>& Bytes);
    float BytesToFloat(const std::vector<int8_t>& Bytes);

    // for comparisons, will convert int8 and int32 to int64
    int64_t BytesAsInt64(const std::vector<int8_t>& Bytes);

    void PrintBytes(const std::vector<int8_t>& Bytes);
    void PrintBytesHex(const std::vector<int8_t>& Bytes);


}


#endif //UISCOINSCRIPTVM_BYTESUTIL_H
