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

#define DEBUGPRINT

// todo checks for script remaining and stack size  / len

bool ScriptExecution::Step() {
    if(StepCounter++ > StepLimit)
        return false;

    if(InstructionCounter >= ScriptBytes.size())
        return false;

    int8_t CurrentOp = ScriptBytes.at(InstructionCounter++);
#ifdef DEBUGPRINT
    std::cout << "Current Op: " << OperatorToName[CurrentOp] << std::endl;
#endif
    switch (CurrentOp){
        case (int8_t)OP_NOP: {
            return true;
        }
        case (int8_t)OP_PUSH: {
            CheckScriptEndsBefore(1);
            int8_t NumberOfBytes = ScriptBytes.at(InstructionCounter++);

            CheckNumberIsInRange(0,INT8_MAX, NumberOfBytes);
            CheckScriptEndsBefore(NumberOfBytes);

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
            CheckScriptEndsBefore(1);
            int8_t Flag = ScriptBytes.at(InstructionCounter++);
            return true;
        }
        case (int8_t)OP_BIGPUSH: {
            CheckScriptEndsBefore(4);
            std::vector<int8_t> NumberOfBytesBytes;
            NumberOfBytesBytes.reserve(4);
            for(int i = 0; i < 4; i++){
                NumberOfBytesBytes.push_back(ScriptBytes.at(InstructionCounter++));
            }

            int32_t NumberOfBytes = BytesUtil::BytesToInt32(NumberOfBytesBytes);

            CheckNumberIsInRange(0,INT32_MAX, NumberOfBytes);
            CheckScriptEndsBefore(NumberOfBytes);

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
            CheckInsufficientStackSize(1);
            auto PickIndexBytes = ScriptStack.back();
            ScriptStack.pop_back();

            int32_t PickIndex;

            if(PickIndexBytes.size() == 1) {
                PickIndex = BytesUtil::BytesToInt8(PickIndexBytes);
            } else {
                PickIndex = BytesUtil::BytesToInt32(PickIndexBytes);
            }

            CheckInsufficientStackSize(PickIndex+1);

            ScriptStack.push_back(ScriptStack.at(PickIndex));

            return true;
        }
        case (int8_t)OP_PUT: {
            CheckInsufficientStackSize(2);
            auto PutIndexBytes = ScriptStack.back();
            ScriptStack.pop_back();

            int32_t PutIndex;

            if(PutIndexBytes.size() == 1) {
                PutIndex = BytesUtil::BytesToInt8(PutIndexBytes);
            } else {
                PutIndex = BytesUtil::BytesToInt32(PutIndexBytes);
            }

            auto PutBytes = ScriptStack.back();
            ScriptStack.pop_back();

            CheckInsufficientStackSize(PutIndex+1);

            ScriptStack.at(PutIndex) = PutBytes;

            return true;
        }
        case (int8_t)OP_NUMEQUAL: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);
            auto B = BytesUtil::BytesAsInt64(BBytes);

#ifdef DEBUGPRINT
    std::cout << A << " == "  << B << " = " << ((A == B)? "true":"false") << std::endl;
#endif

            ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(A == B)));
            return true;
        }
        case (int8_t)OP_BYTESEQUAL: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(ABytes == BBytes)));
            return true;
        }
        case (int8_t)OP_SHA512EQUAL: {
            //return false;
        }
        case (int8_t)OP_LENEQUAL: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(ABytes.size() == BBytes.size())));
            return true;
        }
        case (int8_t)OP_LESSTHAN: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);
            auto B = BytesUtil::BytesAsInt64(BBytes);

            ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(A < B)));
            return true;
        }
        case (int8_t)OP_LESSTHANEQUAL: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);
            auto B = BytesUtil::BytesAsInt64(BBytes);

            ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(A <= B)));
            return true;
        }
        case (int8_t)OP_GREATERTHAN: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);
            auto B = BytesUtil::BytesAsInt64(BBytes);

            ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(A > B)));
            return true;
        }
        case (int8_t)OP_GREATERTHANEQUAL: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);
            auto B = BytesUtil::BytesAsInt64(BBytes);

            ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(A >= B)));
            return true;
        }
        case (int8_t)OP_NOTEQUAL: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(ABytes != BBytes)));
            return true;
        }
        case (int8_t)OP_NOTZERO: {
            CheckInsufficientStackSize(1);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);

            ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(A != 0)));
            return true;
        }
        case (int8_t)OP_GET: {
            // pop len
            //pop begin
            // pop element
            CheckInsufficientStackSize(3);
            auto LenBytes = ScriptStack.back();
            ScriptStack.pop_back();
            auto Len = BytesUtil::BytesAsInt64(LenBytes);
            
            auto BeginIndexBytes = ScriptStack.back();
            ScriptStack.pop_back();
            auto BeginIndex = BytesUtil::BytesAsInt64(BeginIndexBytes);

            auto StackElementIndexBytes = ScriptStack.back();
            ScriptStack.pop_back();
            auto StackElementIndex = BytesUtil::BytesAsInt64(StackElementIndexBytes);

            CheckInsufficientStackSize(StackElementIndex+1);

            std::vector<int8_t> &element = ScriptStack.at(StackElementIndex);

            CheckInsufficientNumberBytes(element,BeginIndex+Len);

            std::vector<int8_t> result(Len);
            std::copy(element.begin()+BeginIndex, element.begin()+BeginIndex+Len, result.begin());

            ScriptStack.push_back(result);
            return true;
        }
        case (int8_t)OP_SET: {
            // pop len
            //pop begin
            // pop element
            //pop value
            CheckInsufficientStackSize(3);
            auto LenBytes = ScriptStack.back();
            ScriptStack.pop_back();
            auto Len = BytesUtil::BytesAsInt64(LenBytes);

            auto BeginIndexBytes = ScriptStack.back();
            ScriptStack.pop_back();
            auto BeginIndex = BytesUtil::BytesAsInt64(BeginIndexBytes);

            auto StackElementIndexBytes = ScriptStack.back();
            ScriptStack.pop_back();
            auto StackElementIndex = BytesUtil::BytesAsInt64(StackElementIndexBytes);

            auto Value = ScriptStack.back();
            ScriptStack.pop_back();

            CheckInsufficientStackSize(StackElementIndex+1);

            CheckInsufficientNumberBytes(Value,Len);
            CheckInsufficientNumberBytes(ScriptStack.at(StackElementIndex),BeginIndex+Len);

            std::copy(Value.begin(), Value.begin()+Len,ScriptStack.at(StackElementIndex).begin()+BeginIndex);

            return true;
        }
        case (int8_t)OP_COPY: {
            //return false;
        }
        case (int8_t)OP_ALLOC: {
            CheckInsufficientStackSize(1);
            auto SizeBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto Size = BytesUtil::BytesAsInt64(SizeBytes);

            ScriptStack.emplace_back(Size, 0);
            return true;
        }
        case (int8_t)OP_THIS: {
            //return false;
        }
        case (int8_t)OP_ADD: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);
            auto B = BytesUtil::BytesAsInt64(BBytes);

            int32_t MaxLen = std::max(ABytes.size(),BBytes.size());

            if(MaxLen == 1){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(A + B)));
                return true;
            } else if(MaxLen == 4){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int32_t)(A + B)));
                return true;
            } else if(MaxLen == 8){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int64_t)(A + B)));
                return true;
            }

            throw std::invalid_argument("Could not auto widen math expression: Len:"+std::to_string(MaxLen));
        }
        case (int8_t)OP_SUBTRACT: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);
            auto B = BytesUtil::BytesAsInt64(BBytes);

            int32_t MaxLen = std::max(ABytes.size(),BBytes.size());

            if(MaxLen == 1){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(A - B)));
                return true;
            } else if(MaxLen == 4){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int32_t)(A - B)));
                return true;
            } else if(MaxLen == 8){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int64_t)(A - B)));
                return true;
            }

            throw std::invalid_argument("Could not auto widen math expression: Len:"+std::to_string(MaxLen));
        }
        case (int8_t)OP_MULTIPLY: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);
            auto B = BytesUtil::BytesAsInt64(BBytes);

            int32_t MaxLen = std::max(ABytes.size(),BBytes.size());

            if(MaxLen == 1){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(A * B)));
                return true;
            } else if(MaxLen == 4){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int32_t)(A * B)));
                return true;
            } else if(MaxLen == 8){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int64_t)(A * B)));
                return true;
            }

            throw std::invalid_argument("Could not auto widen math expression: Len:"+std::to_string(MaxLen));
        }
        case (int8_t)OP_DIVIDE: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);
            auto B = BytesUtil::BytesAsInt64(BBytes);

            int32_t MaxLen = std::max(ABytes.size(),BBytes.size());

            if(B == 0) throw std::invalid_argument("Divide by zero not allowed.");

            if(MaxLen == 1){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(A / B)));
                return true;
            } else if(MaxLen == 4){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int32_t)(A / B)));
                return true;
            } else if(MaxLen == 8){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int64_t)(A / B)));
                return true;
            }

            throw std::invalid_argument("Could not auto widen math expression: Len:"+std::to_string(MaxLen));
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

bool ScriptExecution::Execute() {
    while(Step()) {
#ifdef DEBUGPRINT
        for(const std::vector<int8_t>& vec:ScriptStack){
            BytesUtil::PrintBytes(vec);
            std::cout << std::endl;
        }
#endif
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

void ScriptExecution::CheckScriptEndsBefore(int32_t MinimumRemainingBytes) {
    if(ScriptBytes.size() < InstructionCounter + MinimumRemainingBytes) throw std::logic_error("Expected at least "+std::to_string(MinimumRemainingBytes)+" bytes remaining in the script but there were only "+std::to_string(ScriptBytes.size()-InstructionCounter)+".");
}

void ScriptExecution::CheckInsufficientStackSize(int32_t MinimumSize) {
    if(ScriptStack.size() < MinimumSize) throw std::runtime_error("Expected at least "+std::to_string(MinimumSize)+" elements on the stack but there were only "+std::to_string(ScriptStack.size())+".");
}


void ScriptExecution::CheckNumberIsInRange(int64_t MinInclusive, int64_t MaxInclusive, int64_t Value) {
    if(Value < MinInclusive || Value > MaxInclusive) throw std::runtime_error("Expected value to be "+std::string(Value < MinInclusive ? "greater than "+std::to_string(MinInclusive):"less than"+std::to_string(MaxInclusive))+" but it was "+std::to_string(Value));
}

void ScriptExecution::CheckInsufficientNumberBytes(const std::vector<int8_t>& Value, int32_t MinimumSizeBytes) {
    if(Value.size() < MinimumSizeBytes) throw std::runtime_error("Expected value to be at least "+std::to_string(MinimumSizeBytes)+" bytes but it was "+std::to_string(Value.size()));
}

void ScriptExecution::CheckIncorrectNumberBytes(const std::vector<int8_t>& Value, int32_t ExpectedSizeBytes) {
    if(Value.size() != ExpectedSizeBytes) throw std::runtime_error("Expected value to be "+std::to_string(ExpectedSizeBytes)+" bytes but it was "+std::to_string(Value.size()));
}



