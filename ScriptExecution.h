//
// Created by Admin on 12/7/2020.
//

#ifndef UISCOINSCRIPTVM_SCRIPTEXECUTION_H
#define UISCOINSCRIPTVM_SCRIPTEXECUTION_H

#include <stdint-gcc.h>
#include <vector>


class ScriptExecution {
public:
    int32_t StepCounter = 0;
    int32_t StepLimit = 10000;

    int32_t InstructionCounter = 0;
    std::vector<int8_t> ScriptBytes;

    std::vector<std::vector<int8_t >> ScriptStack;
    bool bScriptFailed = false;

    explicit ScriptExecution(std::vector<int8_t> scriptBytes);
    ScriptExecution(std::vector<int8_t> scriptBytes, std::vector<std::vector<int8_t>> scriptStack);
    ScriptExecution(std::vector<int8_t> scriptBytes, std::vector<std::vector<int8_t>> scriptStack, int32_t stepCounter);

    bool Step();

    bool Execute();

    void CheckScriptEndsBefore(int32_t MinimumRemainingBytes);
    void CheckInsufficientStackSize(int32_t MinimumSize);
    static void CheckInsufficientNumberBytes(const std::vector<int8_t>& Value, int32_t MinimumSizeBytes);
    static void CheckIncorrectNumberBytes(const std::vector<int8_t>& Value, int32_t ExpectedSizeBytes);
    static void CheckNumberIsInRange(int64_t MinInclusive, int64_t MaxInclusive, int64_t Value);
};



#endif //UISCOINSCRIPTVM_SCRIPTEXECUTION_H
