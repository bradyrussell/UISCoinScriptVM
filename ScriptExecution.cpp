//
// Created by Admin on 12/7/2020.
//
#include "ScriptOperator.h"
#include "ScriptExecution.h"
#include "BytesUtil.h"
#include "sha512/sha512.h"
#include "zlib/zlib_wrapper.h"
#include "aes/aes_wrapper.h"
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>

//#define DEBUGPRINT

bool ScriptExecution::Step() {
    if(StepCounter++ > StepLimit) {
        bScriptFailed = true;
       throw std::logic_error("The script has exceeded the execution limit.");
    }

    if(InstructionCounter >= ScriptBytes.size())
        return false;

    int8_t CurrentOp = ScriptBytes.at(InstructionCounter++);
#ifdef DEBUGPRINT
    std::cout << "Current Op: " << OperatorToName[(uint8_t)CurrentOp] << " at Instruction " << std::to_string(InstructionCounter-1) << std::endl;
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

            ScriptStack.push_back(BytesUtil::BooleanToBytes(A == B));
            return true;
        }
        case (int8_t)OP_BYTESEQUAL: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(BytesUtil::BooleanToBytes(ABytes == BBytes));
            return true;
        }
        case (int8_t)OP_SHA512EQUAL: {
            break;
        }
        case (int8_t)OP_LENEQUAL: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(BytesUtil::BooleanToBytes(ABytes.size() == BBytes.size()));
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

            ScriptStack.push_back(BytesUtil::BooleanToBytes(A < B));
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

            ScriptStack.push_back(BytesUtil::BooleanToBytes(A <= B));
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

            ScriptStack.push_back(BytesUtil::BooleanToBytes(A > B));
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

            ScriptStack.push_back(BytesUtil::BooleanToBytes(A >= B));
            return true;
        }
        case (int8_t)OP_NOTEQUAL: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(BytesUtil::BooleanToBytes(ABytes != BBytes));
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
            break;
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
            break;
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
                ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(B - A)));
                return true;
            } else if(MaxLen == 4){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int32_t)(B - A)));
                return true;
            } else if(MaxLen == 8){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int64_t)(B - A)));
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

            if(A == 0) throw std::invalid_argument("Divide by zero not allowed.");

            if(MaxLen == 1){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(B / A)));
                return true;
            } else if(MaxLen == 4){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int32_t)(B / A)));
                return true;
            } else if(MaxLen == 8){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int64_t)(B / A)));
                return true;
            }

            throw std::invalid_argument("Could not auto widen math expression: Len:"+std::to_string(MaxLen));
        }
        case (int8_t)OP_ADDBYTES: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            for(unsigned int i = 0; i < std::min(ABytes.size(),BBytes.size()); i++){
                ABytes.at(i) += BBytes.at(i);
            }

            ScriptStack.push_back(ABytes);
            return true;
        }
        case (int8_t)OP_SUBTRACTBYTES: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            for(unsigned int i = 0; i < std::min(ABytes.size(),BBytes.size()); i++){
                ABytes.at(i) -= BBytes.at(i);
            }

            ScriptStack.push_back(ABytes);
            return true;
        }
        case (int8_t)OP_MULTIPLYBYTES: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            for(unsigned int i = 0; i < std::min(ABytes.size(),BBytes.size()); i++){
                ABytes.at(i) *= BBytes.at(i);
            }

            ScriptStack.push_back(ABytes);
            return true;
        }
        case (int8_t)OP_DIVIDEBYTES: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            for(unsigned int i = 0; i < std::min(ABytes.size(),BBytes.size()); i++){
                if(BBytes.at(i)) throw std::runtime_error("Cannot divide by zero.");
                ABytes.at(i) /= BBytes.at(i);
            }

            ScriptStack.push_back(ABytes);
            return true;
        }
        case (int8_t)OP_NEGATE: {
            CheckInsufficientStackSize(1);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);

            if(ABytes.size() == 1){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(-A)));
                return true;
            } else if(ABytes.size() == 4){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int32_t)(-A)));
                return true;
            } else if(ABytes.size() == 8){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int64_t)(-A)));
                return true;
            }

            throw std::invalid_argument("Could not determine type of math expression: Len:"+std::to_string(ABytes.size()));
        }
        case (int8_t)OP_MODULO: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);
            auto B = BytesUtil::BytesAsInt64(BBytes);

            int32_t MaxLen = std::max(ABytes.size(),BBytes.size());

            if(MaxLen == 1){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)(A % B)));
                return true;
            } else if(MaxLen == 4){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int32_t)(A % B)));
                return true;
            } else if(MaxLen == 8){
                ScriptStack.push_back(BytesUtil::NumberToBytes((int64_t)(A % B)));
                return true;
            }

            throw std::invalid_argument("Could not auto widen math expression: Len:"+std::to_string(MaxLen));
        }
        case (int8_t)OP_CONVERT8TO32: {
            CheckInsufficientStackSize(1);
            auto ValueBytes = ScriptStack.back();
            ScriptStack.pop_back();

            CheckIncorrectNumberBytes(ValueBytes,1);

            ScriptStack.push_back(BytesUtil::NumberToBytes((int32_t)BytesUtil::BytesToInt8(ValueBytes)));
            return true;
        }
        case (int8_t)OP_CONVERT32TO8: {
            CheckInsufficientStackSize(1);
            auto ValueBytes = ScriptStack.back();
            ScriptStack.pop_back();

            CheckIncorrectNumberBytes(ValueBytes,4);

            ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)BytesUtil::BytesToInt32(ValueBytes)));
            return true;
        }
        case (int8_t)OP_CONVERT64TO32: {
            CheckInsufficientStackSize(1);
            auto ValueBytes = ScriptStack.back();
            ScriptStack.pop_back();

            CheckIncorrectNumberBytes(ValueBytes,8);

            ScriptStack.push_back(BytesUtil::NumberToBytes((int32_t)BytesUtil::BytesToInt64(ValueBytes)));
            return true;
        }
        case (int8_t)OP_CONVERT32TO64: {
            CheckInsufficientStackSize(1);
            auto ValueBytes = ScriptStack.back();
            ScriptStack.pop_back();

            CheckIncorrectNumberBytes(ValueBytes,4);

            ScriptStack.push_back(BytesUtil::NumberToBytes((int64_t)BytesUtil::BytesToInt32(ValueBytes)));
            return true;
        }
        case (int8_t)OP_CONVERTFLOATTO32: {
            CheckInsufficientStackSize(1);
            auto ValueBytes = ScriptStack.back();
            ScriptStack.pop_back();

            CheckIncorrectNumberBytes(ValueBytes,4);

            ScriptStack.push_back(BytesUtil::NumberToBytes((int32_t)BytesUtil::BytesToFloat(ValueBytes)));
            return true;
        }
        case (int8_t)OP_CONVERT32TOFLOAT: {
            CheckInsufficientStackSize(1);
            auto ValueBytes = ScriptStack.back();
            ScriptStack.pop_back();

            CheckIncorrectNumberBytes(ValueBytes,4);

            ScriptStack.push_back(BytesUtil::NumberToBytes((float)BytesUtil::BytesToInt32(ValueBytes)));
            return true;
        }
        case (int8_t)OP_BITNOT: {
            break;
        }
        case (int8_t)OP_BITOR: {
            break;
        }
        case (int8_t)OP_BITAND: {
            break;
        }
        case (int8_t)OP_BITXOR: {
            break;
        }
        case (int8_t)OP_APPEND: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ABytes.insert(std::end(ABytes), std::begin(BBytes), std::end(BBytes));
            ScriptStack.push_back(ABytes);
            return true;
        }
        case (int8_t)OP_LIMIT: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);

            CheckNumberIsInRange(0,ScriptStack.back().size(), A);

            ScriptStack.back().resize(A);
            return true;
        }
        case (int8_t)OP_REVERSE: {
            CheckInsufficientStackSize(1);
            std::reverse(std::begin(ScriptStack.back()), std::end(ScriptStack.back()));
            return true;
        }
        case (int8_t)OP_SPLIT: {
            CheckInsufficientStackSize(1);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            for(auto byte:ABytes){
                ScriptStack.emplace_back(1,byte);
            }

            break;
        }
        case (int8_t)OP_COMBINE: {
            break;
        }
        case (int8_t)OP_LEN: {
            CheckInsufficientStackSize(1);
            auto ValueBytes = ScriptStack.back();
            //ScriptStack.pop_back(); // todo dont think len pops

            ScriptStack.push_back(BytesUtil::NumberToBytes((int32_t)ValueBytes.size())); // todo uses int?
            return true;
        }
        case (int8_t)OP_NOT: {
            CheckInsufficientStackSize(1);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(BytesUtil::BooleanToBytes(!BytesUtil::BytesToBoolean(ABytes))); // strict check for 1
            return true;
        }
        case (int8_t)OP_OR: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(BytesUtil::BooleanToBytes(BytesUtil::BytesToBoolean(ABytes) || BytesUtil::BytesToBoolean(BBytes)));
            return true;
        }
        case (int8_t)OP_AND: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(BytesUtil::BooleanToBytes(BytesUtil::BytesToBoolean(ABytes) && BytesUtil::BytesToBoolean(BBytes)));
            return true;
        }
        case (int8_t)OP_XOR: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(BytesUtil::BooleanToBytes((BytesUtil::BytesToBoolean(ABytes) || BytesUtil::BytesToBoolean(BBytes)) && !(BytesUtil::BytesToBoolean(ABytes) && BytesUtil::BytesToBoolean(BBytes))));
            return true;
        }
        case (int8_t)OP_INVERTFLOAT: {
            break;
        }
        case (int8_t)OP_NEGATEFLOAT: {
            break;
        }
        case (int8_t)OP_ADDFLOAT: {
            break;
        }
        case (int8_t)OP_SUBTRACTFLOAT: {
            break;
        }
        case (int8_t)OP_MULTIPLYFLOAT: {
            break;
        }
        case (int8_t)OP_DIVIDEFLOAT: {
            break;
        }
        case (int8_t)OP_NULL: {
            ScriptStack.emplace_back(0);
            return true;
        }
        case (int8_t)OP_FALSE: {
            ScriptStack.emplace_back(1,0);
            return true;
        }
        case (int8_t)OP_TRUE: {
            ScriptStack.emplace_back(1,1);
            return true;
        }
        case (int8_t)OP_DEPTH: {
            ScriptStack.push_back(BytesUtil::NumberToBytes((int8_t)ScriptStack.size())); // todo check if this returns int32 or int8 in java
            return true;
        }
        case (int8_t)OP_DROP: {
            CheckInsufficientStackSize(1);
            ScriptStack.pop_back();
            return true;
        }
        case (int8_t)OP_DUP: {
            CheckInsufficientStackSize(1);
            ScriptStack.push_back(ScriptStack.back());
            return true;
        }
        case (int8_t)OP_SWAP: {
            CheckInsufficientStackSize(2);

            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(ABytes);
            ScriptStack.push_back(BBytes);
            return true;
        }
        case (int8_t)OP_CLEAR: {
            ScriptStack.clear();
            return true;
        }
        case (int8_t)OP_CLONE: {
            ScriptStack.insert(std::end(ScriptStack), std::begin(ScriptStack), std::end(ScriptStack));
            return true;
        }
        case (int8_t)OP_FLIP: {
            std::reverse(std::begin(ScriptStack), std::end(ScriptStack));
            return true;
        }
        case (int8_t)OP_SHIFTUP: {
            std::rotate(std::begin(ScriptStack), std::begin(ScriptStack) + ScriptStack.size() - 1, std::end(ScriptStack));
            return true;
        }
        case (int8_t)OP_SHIFTDOWN: {
            std::rotate(std::begin(ScriptStack), std::begin(ScriptStack) + 1, std::end(ScriptStack));
            return true;
        }
        case (int8_t)OP_SHIFTN: { // todo crashes // todo middle param is : element within the range [first,last) that is moved to the first position in the range. NOT amount of positions to shift
            CheckInsufficientStackSize(1);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);
            std::rotate(std::begin(ScriptStack), std::begin(ScriptStack) + A, std::end(ScriptStack)); // todo can this handle like -498573?
            return true;
        }
        case (int8_t)OP_SHIFTELEMENTSRIGHT: {
            CheckInsufficientStackSize(1);
            std::rotate(std::begin(ScriptStack.back()), std::begin(ScriptStack.back()) + 1, std::end(ScriptStack.back()));
            return true;
        }
        case (int8_t)OP_SHIFTELEMENTSLEFT: {
            CheckInsufficientStackSize(1);
            std::rotate(std::begin(ScriptStack.back()), std::begin(ScriptStack.back()) - 1, std::end(ScriptStack.back()));
            return true;
        }
        case (int8_t)OP_DUP2: {
            CheckInsufficientStackSize(2);

            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(BBytes);
            ScriptStack.push_back(BBytes);
            ScriptStack.push_back(ABytes);
            ScriptStack.push_back(ABytes);
            return true;
        }
        case (int8_t)OP_DUPN: {
            CheckInsufficientStackSize(1);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);

            CheckNumberIsInRange(0,ScriptStack.size()-1, A);
            CheckInsufficientStackSize(A);

            ScriptStack.insert(std::end(ScriptStack), std::end(ScriptStack)-A, std::end(ScriptStack)); // todo see if this works
            return true;
        }
        case (int8_t)OP_DROPN: {
            CheckInsufficientStackSize(1);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto A = BytesUtil::BytesAsInt64(ABytes);

            CheckNumberIsInRange(0,ScriptStack.size()-1, A);
            CheckInsufficientStackSize(A);

            ScriptStack.resize(ScriptStack.size() - A); // todo check if this works
            return true;
        }
        case (int8_t)OP_SHIFTNEXCEPT: {
            break;
        }
        case (int8_t)OP_VERIFY: {
            CheckInsufficientStackSize(1);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            return BytesUtil::BytesToBoolean(ABytes); // strict check for 1
        }
        case (int8_t)OP_RETURN: {
            return false;
        }
        case (int8_t)OP_RETURNIF: {
            CheckInsufficientStackSize(1);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            return !BytesUtil::BytesToBoolean(ABytes); // strict check for 1
        }
        case (int8_t)OP_SHA512: {
            CheckInsufficientStackSize(1);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(sha512(ABytes));
            return true;
        }
        case (int8_t)OP_ZIP: {
            CheckInsufficientStackSize(1);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(zlib::zip(ABytes));
            return true;
        }
        case (int8_t)OP_UNZIP: {
            CheckInsufficientStackSize(1);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(zlib::unzip(ABytes));
            return true;
        }
        case (int8_t)OP_ENCRYPTAES: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(aes::encrypt(ABytes, BBytes));
            return true;
        }
        case (int8_t)OP_DECRYPTAES: {
            CheckInsufficientStackSize(2);
            auto ABytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto BBytes = ScriptStack.back();
            ScriptStack.pop_back();

            ScriptStack.push_back(aes::decrypt(ABytes, BBytes));
            return true;
        }
        case (int8_t)OP_VERIFYSIG: {
            //this one is probably gonna require openssl
           // https://github.com/gladosconn/ecdsa_cxx ?
            break;
        }
        case (int8_t)OP_CODESEPARATOR: {
            break;
        }
        case (int8_t)OP_CALL: {
            //pop script
            //pop #
            //pop that # of elems

            CheckInsufficientStackSize(2);
            auto VirtualScriptBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto StackElementNumberBytes = ScriptStack.back();
            ScriptStack.pop_back();

            auto StackElementNumber = BytesUtil::BytesAsInt64(StackElementNumberBytes);

            CheckInsufficientStackSize(StackElementNumber);

            ScriptExecution callExecution = ScriptExecution(VirtualScriptBytes, std::vector<std::vector<int8_t>>(ScriptStack.end()-StackElementNumber, ScriptStack.end()), StepCounter);
            ScriptStack.resize(ScriptStack.size() - StackElementNumber); // pop N elements

            bool callResult = callExecution.Execute();

            StepCounter = callExecution.StepCounter+1; // count steps inside call

            ScriptStack.insert(std::end(ScriptStack), std::begin(callExecution.ScriptStack), std::end(callExecution.ScriptStack)); // copy resulting stack onto parent stack

            ScriptStack.push_back(BytesUtil::BooleanToBytes(callResult));
            return true;
        }
        case (int8_t)OP_JUMP: {
            break;
        }
        case (int8_t)OP_JUMPIF: {
            break;
        }
        default:{
            break;
        }
    }
    std::cout << "Unhandled OP: " << OperatorToName[(uint8_t)CurrentOp] << std::endl;
    bScriptFailed = true;
    return false;
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
    if(ScriptStack.size() < MinimumSize) {
#ifdef DEBUGPRINT
        std::cout << "Expected at least "+std::to_string(MinimumSize)+" elements on the stack but there were only "+std::to_string(ScriptStack.size())+"."  << std::endl;
#endif
        throw std::runtime_error("Expected at least "+std::to_string(MinimumSize)+" elements on the stack but there were only "+std::to_string(ScriptStack.size())+".");
    }

}


void ScriptExecution::CheckNumberIsInRange(int64_t MinInclusive, int64_t MaxInclusive, int64_t Value) {
    if(Value < MinInclusive || Value > MaxInclusive) {
#ifdef DEBUGPRINT
        std::cout << "Expected value to be "+std::string(Value < MinInclusive ? "greater than "+std::to_string(MinInclusive):"less than"+std::to_string(MaxInclusive))+" but it was "+std::to_string(Value)  << std::endl;
#endif
        throw std::runtime_error("Expected value to be "+std::string(Value < MinInclusive ? "greater than "+std::to_string(MinInclusive):"less than"+std::to_string(MaxInclusive))+" but it was "+std::to_string(Value));
    }
}

void ScriptExecution::CheckInsufficientNumberBytes(const std::vector<int8_t>& Value, int32_t MinimumSizeBytes) {
    if(Value.size() < MinimumSizeBytes) {
#ifdef DEBUGPRINT
        std::cout << "Expected value to be at least "+std::to_string(MinimumSizeBytes)+" bytes but it was "+std::to_string(Value.size())  << std::endl;
#endif
        throw std::runtime_error("Expected value to be at least "+std::to_string(MinimumSizeBytes)+" bytes but it was "+std::to_string(Value.size()));
    }
}

void ScriptExecution::CheckIncorrectNumberBytes(const std::vector<int8_t>& Value, int32_t ExpectedSizeBytes) {
    if(Value.size() != ExpectedSizeBytes) {
#ifdef DEBUGPRINT
        std::cout << "Expected value to be "+std::to_string(ExpectedSizeBytes)+" bytes but it was "+std::to_string(Value.size())  << std::endl;
#endif
        throw std::runtime_error("Expected value to be "+std::to_string(ExpectedSizeBytes)+" bytes but it was "+std::to_string(Value.size()));
    }
}



