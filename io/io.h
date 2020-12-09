//
// Created by Admin on 12/8/2020.
//

#ifndef UISCOINSCRIPTVM_IO_H
#define UISCOINSCRIPTVM_IO_H

#include <vector>
#include <stdint-gcc.h>
#include <fstream>

namespace io{
    std::vector<int8_t> readAllBytes(const char *Path) {
        std::ifstream input(Path, std::ios::binary);

        if(!input.is_open() || input.fail())
            return std::vector<int8_t>();

        std::vector<int8_t> bytes(
                (std::istreambuf_iterator<char>(input)),
                (std::istreambuf_iterator<char>()));

        input.close();

        return bytes;
    }

    bool writeAllBytes(const char* Path, std::vector<int8_t> Bytes){
        std::ofstream output(Path, std::ios::out | std::ios::binary);
        output.write(reinterpret_cast<const char *>(Bytes.data()), Bytes.size());
        bool fail = output.fail();
        output.close();
        return fail;
    }
}

#endif //UISCOINSCRIPTVM_IO_H
