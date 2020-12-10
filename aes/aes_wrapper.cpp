//
// Created by Admin on 12/9/2020.
//

#include "aes_wrapper.h"
#include "../BytesUtil.h"
#include "aes.h"

std::vector<int8_t> aes::encrypt(const std::vector<int8_t>& data, const std::vector<int8_t>& key) {
    const std::vector<unsigned char> computed_key = aes::key_from_hash(key);

    const unsigned long encrypted_size = aes::get_padded_encrypted_size(data.size());
    std::vector<unsigned char> encrypted(encrypted_size);

    aes::encrypt_ecb((unsigned char*)data.data(), data.size(), computed_key.data(), computed_key.size(), encrypted.data(), encrypted.size(), true);

    return std::vector<int8_t>(encrypted.begin(), encrypted.end());
}

std::vector<int8_t> aes::decrypt(const std::vector<int8_t>& data, const std::vector<int8_t>& key) {
    const std::vector<unsigned char> computed_key = aes::key_from_hash(key);

    unsigned long padded_size = 0;
    std::vector<unsigned char> decrypted(data.size());

    aes::decrypt_ecb((unsigned char*)data.data(), data.size(), computed_key.data(), computed_key.size(), decrypted.data(), decrypted.size(), &padded_size);

    std::vector<int8_t > decryptedbytes(decrypted.begin(), decrypted.end());

    decryptedbytes.resize(decryptedbytes.size()-padded_size);
    return decryptedbytes;
}
