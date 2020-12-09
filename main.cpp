#include <iostream>
#include <sstream>
#include <chrono>
#include "ScriptOperator.h"
#include "ScriptExecution.h"
#include "BytesUtil.h"
#include "crypto/sha512.h"
#include "zlib/zlib_wrapper.h"
#include <zlib.h>



int main() {
    auto beginTime = std::chrono::steady_clock::now();
    try {
        int8_t Script[] = {-127, -127, -127, -127, -127, -127, 1, 1, 10, 42, 1, 1, 0, 8, 1, 1, 5, 42, 1, 1, 1, 8, 1, 9, 1, 1, 5, 7, 1, 1, 4, 7, 33, 1, 1, 3, 8, 1, 1, 0, 7, 1, 1, 4, 8, 1, 1, 1, 7, 1, 1, 5, 8, -112, 1, 1, 3, 7, -48, -96, 1, 1, 2, 8};
        std::vector<int8_t > scriptVector;
        scriptVector.insert(scriptVector.begin(), std::begin(Script), std::end(Script));

        ScriptExecution scriptExecution(scriptVector);

        bool success = scriptExecution.Execute();

        std::cout << "Script execution returned: " << (success ? "true" : "false") << std::endl;
    } catch (std::exception& e){
        std::cout << "Caught exception!" << std::endl;
        std::cout << e.what();
        return 1;
    } catch (...){
        std::cout << "Unhandled exception!" << std::endl;
        return 2;
    }
    auto endTime = std::chrono::steady_clock::now();
    std::chrono::nanoseconds elapsedNs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - beginTime);
    std::cout <<  " in "  << elapsedNs.count()/1000000. << " ms." << std::endl;
    return 0;
}