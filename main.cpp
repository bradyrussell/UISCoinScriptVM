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
        int8_t Script[] = {1, 1, 97, 1, 1, 98, 1, 1, 99, 1, 1, 21, -103};
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