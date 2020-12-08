#include <iostream>
#include <sstream>
#include "ScriptOperator.h"
#include "ScriptExecution.h"
#include "BytesUtil.h"


//OperatorToName[OP_CALL]

int main() {
    try {


        int8_t Script[] = {1,
                           4,
                           0,
                           0,
                           0,
                           1,
                           1,
                           4,
                           0,
                           0,
                           0,
                           2,
                           1,
                           1,
                           0,
                           7,
                           1,
                           1,
                           1,
                           8};

        std::vector<int8_t > scriptVector;
        scriptVector.insert(scriptVector.begin(), std::begin(Script), std::end(Script));

        ScriptExecution scriptExecution(scriptVector);

        bool success = scriptExecution.Execute(true);

        std::cout << "Script execution returned: " << (success ? "true" : "false") << std::endl;
        return 0;
    } catch (std::exception& e){
        std::cout << e.what();
    }
}