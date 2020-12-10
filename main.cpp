#include <iostream>
#include <chrono>
#include "ScriptOperator.h"
#include "ScriptExecution.h"


int main() {

    //auto scriptFromFile = io::readAllBytes("..\\compiled.uiscb");

   // BytesUtil::PrintBytes(scriptFromFile);

    auto beginTime = std::chrono::steady_clock::now();
    try {
        int8_t Script[] = {1, 4, 0, 0, 0, 1, 1, 4, 0, 0, 0, 2, -75, 1, 4, 0, 0, 0, 1, -109, -74};
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