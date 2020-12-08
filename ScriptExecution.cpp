//
// Created by Admin on 12/7/2020.
//
#include "ScriptOperator.h"
#include "ScriptExecution.h"
#include "BytesUtil.h"
//#include <utility>
#include <iostream>
#include <utility>
#include <sstream>
#include <iterator>

// todo checks for script remaining and stack size  / len

bool ScriptExecution::Step() {
    if(StepCounter++ > StepLimit)
        return false;

    if(InstructionCounter >= ScriptBytes.size())
        return false;

    int8_t CurrentOp = ScriptBytes.at(InstructionCounter++);

    std::cout << "Current Op: " << OperatorToName[CurrentOp] << std::endl;

    switch (CurrentOp){
        case (int8_t)OP_NOP: {
            return true;
        }
        case (int8_t)OP_PUSH: {
            int8_t NumberOfBytes = ScriptBytes.at(InstructionCounter++);

            std::vector<int8_t> PushData;

            PushData.reserve(NumberOfBytes);
            for(int i = 0; i < NumberOfBytes; i++) {
                PushData.emplace_back(ScriptBytes.at(InstructionCounter++));
            }

            ScriptStack.push_back(PushData);
            return true;
        }
        case (int8_t)OP_INSTRUCTION: {
            ScriptStack.push_back(BytesUtil::NumberToBytes(InstructionCounter));
            return true;
        }
        case (int8_t)OP_FLAG: {
            int8_t Flag = ScriptBytes.at(InstructionCounter++);
            return true;
        }
        case (int8_t)OP_BIGPUSH: {
            std::vector<int8_t> NumberOfBytesBytes;
            NumberOfBytesBytes.reserve(4);
            for(int i = 0; i < 4; i++){
                NumberOfBytesBytes.push_back(ScriptBytes.at(InstructionCounter++));
            }

            int32_t NumberOfBytes = BytesUtil::BytesToInt32(NumberOfBytesBytes);

            std::vector<int8_t> PushData;

            PushData.reserve(NumberOfBytes);
            for(int i = 0; i < NumberOfBytes; i++) {
                PushData.emplace_back(ScriptBytes.at(InstructionCounter++));
            }

            ScriptStack.push_back(PushData);
            return true;
        }
        case (int8_t)OP_FLAGDATA: {
            int8_t NumberOfBytes = ScriptBytes.at(InstructionCounter++);

            std::vector<int8_t> FlagData;

            FlagData.reserve(NumberOfBytes);
            for(int i = 0; i < NumberOfBytes; i++) {
                FlagData.emplace_back(ScriptBytes.at(InstructionCounter++));
            }

            return true;
        }
        case (int8_t)OP_TIME: {
            return false;
        }
        case (int8_t)OP_PICK: {
            auto PickIndexBytes = ScriptStack.back();
            ScriptStack.pop_back();

            int32_t PickIndex = -1;

            if(PickIndexBytes.size() == 1) {
                PickIndex = BytesUtil::BytesToInt8(PickIndexBytes);
            } else {
                PickIndex = BytesUtil::BytesToInt32(PickIndexBytes);
            }

            ScriptStack.push_back(ScriptStack.at(PickIndex)); // todo check out of bounds

            return true;
        }
        case (int8_t)OP_PUT: {
            auto PutIndexBytes = ScriptStack.back();
            ScriptStack.pop_back();

            int32_t PutIndex = -1;

            if(PutIndexBytes.size() == 1) {
                PutIndex = BytesUtil::BytesToInt8(PutIndexBytes);
            } else {
                PutIndex = BytesUtil::BytesToInt32(PutIndexBytes);
            }

            auto PutBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.at(PutIndex) = PutBytes;

            return true;
        }
        case (int8_t)OP_NUMEQUAL: {
            //return false;
        }
        case (int8_t)OP_BYTESEQUAL: {
            //return false;
        }
        case (int8_t)OP_SHA512EQUAL: {
            //return false;
        }
        case (int8_t)OP_LENEQUAL: {
            //return false;
        }
        case (int8_t)OP_LESSTHAN: {
            //return false;
        }
        case (int8_t)OP_LESSTHANEQUAL: {
            //return false;
        }
        case (int8_t)OP_GREATERTHAN: {
            //return false;
        }
        case (int8_t)OP_GREATERTHANEQUAL: {
            //return false;
        }
        case (int8_t)OP_NOTEQUAL: {
            //return false;
        }
        case (int8_t)OP_NOTZERO: {
            //return false;
        }
        case (int8_t)OP_GET: {
            //return false;
        }
        case (int8_t)OP_SET: {
            //return false;
        }
        case (int8_t)OP_COPY: {
            //return false;
        }
        case (int8_t)OP_ALLOC: {
            //return false;
        }
        case (int8_t)OP_THIS: {
            //return false;
        }
        case (int8_t)OP_ADD: {
            //return false;
        }
        case (int8_t)OP_SUBTRACT: {
            //return false;
        }
        case (int8_t)OP_MULTIPLY: {
            //return false;
        }
        case (int8_t)OP_DIVIDE: {
            //return false;
        }
        case (int8_t)OP_ADDBYTES: {
            //return false;
        }
        case (int8_t)OP_SUBTRACTBYTES: {
            //return false;
        }
        case (int8_t)OP_MULTIPLYBYTES: {
            //return false;
        }
        case (int8_t)OP_DIVIDEBYTES: {
            //return false;
        }
        case (int8_t)OP_NEGATE: {
            //return false;
        }
        case (int8_t)OP_MODULO: {
            //return false;
        }
        case (int8_t)OP_CONVERT8TO32: {
            //return false;
        }
        case (int8_t)OP_CONVERT32TO8: {
            //return false;
        }
        case (int8_t)OP_CONVERT64TO32: {
            //return false;
        }
        case (int8_t)OP_CONVERT32TO64: {
            //return false;
        }
        case (int8_t)OP_CONVERTFLOATTO32: {
            //return false;
        }
        case (int8_t)OP_CONVERT32TOFLOAT: {
            //return false;
        }
        case (int8_t)OP_BITNOT: {
            //return false;
        }
        case (int8_t)OP_BITOR: {
            //return false;
        }
        case (int8_t)OP_BITAND: {
            //return false;
        }
        case (int8_t)OP_BITXOR: {
            //return false;
        }
        case (int8_t)OP_APPEND: {
            //return false;
        }
        case (int8_t)OP_LIMIT: {
            //return false;
        }
        case (int8_t)OP_REVERSE: {
            //return false;
        }
        case (int8_t)OP_SPLIT: {
            //return false;
        }
        case (int8_t)OP_COMBINE: {
            //return false;
        }
        case (int8_t)OP_LEN: {
            //return false;
        }
        case (int8_t)OP_NOT: {
            //return false;
        }
        case (int8_t)OP_OR: {
            //return false;
        }
        case (int8_t)OP_AND: {
            //return false;
        }
        case (int8_t)OP_XOR: {
            //return false;
        }
        case (int8_t)OP_INVERTFLOAT: {
            //return false;
        }
        case (int8_t)OP_NEGATEFLOAT: {
            //return false;
        }
        case (int8_t)OP_ADDFLOAT: {
            //return false;
        }
        case (int8_t)OP_SUBTRACTFLOAT: {
            //return false;
        }
        case (int8_t)OP_MULTIPLYFLOAT: {
            //return false;
        }
        case (int8_t)OP_DIVIDEFLOAT: {
            //return false;
        }
        case (int8_t)OP_NULL: {
            //return false;
        }
        case (int8_t)OP_FALSE: {
            //return false;
        }
        case (int8_t)OP_TRUE: {
            //return false;
        }
        case (int8_t)OP_DEPTH: {
            //return false;
        }
        case (int8_t)OP_DROP: {
            //return false;
        }
        case (int8_t)OP_DUP: {
            //return false;
        }
        case (int8_t)OP_SWAP: {
            //return false;
        }
        case (int8_t)OP_CLEAR: {
            //return false;
        }
        case (int8_t)OP_CLONE: {
            //return false;
        }
        case (int8_t)OP_FLIP: {
            //return false;
        }
        case (int8_t)OP_SHIFTUP: {
            //return false;
        }
        case (int8_t)OP_SHIFTDOWN: {
            //return false;
        }
        case (int8_t)OP_SHIFTN: {
            //return false;
        }
        case (int8_t)OP_SHIFTELEMENTSRIGHT: {
            //return false;
        }
        case (int8_t)OP_SHIFTELEMENTSLEFT: {
            //return false;
        }
        case (int8_t)OP_DUP2: {
            //return false;
        }
        case (int8_t)OP_DUPN: {
            //return false;
        }
        case (int8_t)OP_DROPN: {
            //return false;
        }
        case (int8_t)OP_SHIFTNEXCEPT: {
            //return false;
        }
        case (int8_t)OP_VERIFY: {
            //return false;
        }
        case (int8_t)OP_RETURN: {
            //return false;
        }
        case (int8_t)OP_RETURNIF: {
            //return false;
        }
        case (int8_t)OP_SHA512: {
            //return false;
        }
        case (int8_t)OP_ZIP: {
            //return false;
        }
        case (int8_t)OP_UNZIP: {
            //return false;
        }
        case (int8_t)OP_ENCRYPTAES: {
            //return false;
        }
        case (int8_t)OP_DECRYPTAES: {
            //return false;
        }
        case (int8_t)OP_VERIFYSIG: {
            //return false;
        }
        case (int8_t)OP_CODESEPARATOR: {
            //return false;
        }
        case (int8_t)OP_CALL: {
            //return false;
        }
        case (int8_t)OP_JUMP: {
            //return false;
        }
        case (int8_t)OP_JUMPIF: {
            //return false;
        }
        default:{
            std::cout << "Unhandled OP: " << OperatorToName[CurrentOp] << std::endl;
            bScriptFailed = true;
            return false;
        }
    }
}

bool ScriptExecution::Execute(bool bDebugPrint) {
    while(Step()) {
        if(bDebugPrint) {
            for(const std::vector<int8_t>& vec:ScriptStack){
                BytesUtil::PrintBytes(vec);
                std::cout << std::endl;
            }
        }
    }
    return !bScriptFailed;
}

ScriptExecution::ScriptExecution(std::vector<int8_t> scriptBytes) : ScriptBytes(std::move(scriptBytes)) {}

ScriptExecution::ScriptExecution(std::vector<int8_t> scriptBytes,
                                 std::vector<std::vector<int8_t>> scriptStack) : ScriptBytes(std::move(scriptBytes)),
                                                                                        ScriptStack(std::move(scriptStack)) {}

ScriptExecution::ScriptExecution( std::vector<int8_t> scriptBytes,
                                 std::vector<std::vector<int8_t>> scriptStack,int32_t stepCounter) : StepCounter(stepCounter),
                                                                                        ScriptBytes(std::move(scriptBytes)),
                                                                                        ScriptStack(std::move(scriptStack)) {}



