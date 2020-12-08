#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
//
// Created by Admin on 12/7/2020.
//
#include <algorithm>
#include <cstdio>
#include <stdexcept>
#include "BytesUtil.h"



void BytesUtil::PrintBytesHex(std::vector<int8_t> Bytes) {
    for(int8_t i:Bytes){
        printf("%x ", i & 0xff);
    }
}

void BytesUtil::PrintBytes(std::vector<int8_t> Bytes) {
    for(int8_t i:Bytes){
        printf("%d ", i);
    }
}

std::vector<int8_t> BytesUtil::NumberToBytes(int8_t Number) {
    return std::vector<int8_t>(1, Number);
}

std::vector<int8_t> BytesUtil::NumberToBytes(int32_t Number) {
    std::vector<int8_t> bytes;

    bytes.reserve(4);
    bytes.push_back ((int8_t)(Number >> 24));
    bytes.push_back ((int8_t)(Number >> 16));
    bytes.push_back ((int8_t)(Number >> 8));
    bytes.push_back ((int8_t)Number);

    return bytes;
}

std::vector<int8_t> BytesUtil::NumberToBytes(int64_t Number) {
    std::vector<int8_t> bytes;

    bytes.reserve(8);
    bytes.push_back ((int8_t)(Number >> 56));
    bytes.push_back ((int8_t)(Number >> 48));
    bytes.push_back ((int8_t)(Number >> 40));
    bytes.push_back ((int8_t)(Number >> 32));
    bytes.push_back ((int8_t)(Number >> 24));
    bytes.push_back ((int8_t)(Number >> 16));
    bytes.push_back ((int8_t)(Number >> 8));
    bytes.push_back ((int8_t)Number);

    return bytes;
}

std::vector<int8_t> BytesUtil::NumberToBytes(float Number) { // todo this is prob not valid on all platforms
    int32_t bits =  *reinterpret_cast<int32_t*>(&Number);
    return NumberToBytes(bits);
}

int8_t BytesUtil::BytesToInt8(const std::vector<int8_t>& Bytes) {
    if(Bytes.size() != 1) throw std::invalid_argument("BytesToInt8 expected byte array of length 1, found "+std::to_string(Bytes.size()));
    return Bytes.at(0);
}

int32_t BytesUtil::BytesToInt32(const std::vector<int8_t>& Bytes) {
    if(Bytes.size() != 4) throw std::invalid_argument("BytesToInt32 expected byte array of length 4, found "+std::to_string(Bytes.size()));
    return ((int32_t) Bytes.at(0) & 0xFF) << 24
           | ((int32_t) Bytes.at(1) & 0xFF) << 16
           | ((int32_t) Bytes.at(2) & 0xFF) << 8
           | ((int32_t) Bytes.at(3) & 0xFF);
}

int64_t BytesUtil::BytesToInt64(const std::vector<int8_t>& Bytes) {
    if(Bytes.size() != 8) throw std::invalid_argument("BytesToInt64 expected byte array of length 8, found "+std::to_string(Bytes.size()));
    return ((int64_t) Bytes.at(0) << 56)
           | ((int64_t) Bytes.at(1) & 0xFF) << 48
           | ((int64_t) Bytes.at(2) & 0xFF) << 40
           | ((int64_t) Bytes.at(3) & 0xFF) << 32
           | ((int64_t) Bytes.at(4) & 0xFF) << 24
           | ((int64_t) Bytes.at(5) & 0xFF) << 16
           | ((int64_t) Bytes.at(6) & 0xFF) << 8
           | ((int64_t) Bytes.at(7) & 0xFF);
}

float BytesUtil::BytesToFloat(const std::vector<int8_t>& Bytes) {
    if(Bytes.size() != 4) throw std::invalid_argument("BytesToFloat expected byte array of length 4, found "+std::to_string(Bytes.size()));
    int32_t  bits = BytesToInt32(Bytes);
    return  *reinterpret_cast<float*>(&bits);
}

int64_t BytesUtil::BytesAsInt64(const std::vector<int8_t> &Bytes) {
    if(Bytes.size() == 1) return BytesToInt8(Bytes);
    if(Bytes.size() == 4) return BytesToInt32(Bytes);
    if(Bytes.size() == 8) return BytesToInt64(Bytes);
    throw std::invalid_argument("BytesAsInt64 expected byte array of length 1, 4 or 8, found "+std::to_string(Bytes.size()));
}

bool BytesUtil::BytesToBoolean(const std::vector<int8_t> &Bytes) {
    return !(Bytes.size() == 1 && Bytes.at(0) == 0); // todo less strict checking?
}

std::vector<int8_t> BytesUtil::BooleanToBytes(bool Boolean) {
    return std::vector<int8_t>(1, Boolean);
}

#pragma clang diagnostic pop