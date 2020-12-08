//
// Created by Admin on 12/7/2020.
//

#ifndef UISCOINSCRIPTVM_SCRIPTOPERATOR_H
#define UISCOINSCRIPTVM_SCRIPTOPERATOR_H
static const char* OperatorToName[] = { "OP_NOP", "OP_PUSH", "OP_INSTRUCTION", "OP_FLAG", "OP_BIGPUSH", "OP_FLAGDATA", "OP_TIME", "OP_PICK", "OP_PUT", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "OP_NUMEQUAL", "OP_BYTESEQUAL", "OP_SHA512EQUAL", "OP_LENEQUAL", "OP_LESSTHAN", "OP_LESSTHANEQUAL", "OP_GREATERTHAN", "OP_GREATERTHANEQUAL", "OP_NOTEQUAL", "OP_NOTZERO", "OP_GET", "OP_SET", "OP_COPY", "OP_ALLOC", "OP_THIS", "Invalid Operator", "OP_ADD", "OP_SUBTRACT", "OP_MULTIPLY", "OP_DIVIDE", "OP_ADDBYTES", "OP_SUBTRACTBYTES", "OP_MULTIPLYBYTES", "OP_DIVIDEBYTES", "OP_NEGATE", "OP_MODULO", "OP_CONVERT8TO32", "OP_CONVERT32TO8", "OP_CONVERT64TO32", "OP_CONVERT32TO64", "OP_CONVERTFLOATTO32", "OP_CONVERT32TOFLOAT", "Invalid Operator", "OP_BITNOT", "OP_BITOR", "OP_BITAND", "OP_BITXOR", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "OP_APPEND", "OP_LIMIT", "OP_REVERSE", "OP_SPLIT", "OP_COMBINE", "OP_LEN", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "OP_NOT", "OP_OR", "OP_AND", "OP_XOR", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "OP_INVERTFLOAT", "OP_NEGATEFLOAT", "OP_ADDFLOAT", "OP_SUBTRACTFLOAT", "OP_MULTIPLYFLOAT", "OP_DIVIDEFLOAT", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "OP_NULL", "OP_FALSE", "OP_TRUE", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "OP_DEPTH", "OP_DROP", "OP_DUP", "OP_SWAP", "OP_CLEAR", "OP_CLONE", "OP_FLIP", "OP_SHIFTUP", "OP_SHIFTDOWN", "OP_SHIFTN", "OP_SHIFTELEMENTSRIGHT", "OP_SHIFTELEMENTSLEFT", "OP_DUP2", "OP_DUPN", "OP_DROPN", "OP_SHIFTNEXCEPT", "OP_VERIFY", "OP_RETURN", "OP_RETURNIF", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "OP_SHA512", "OP_ZIP", "OP_UNZIP", "Invalid Operator", "Invalid Operator", "OP_ENCRYPTAES", "OP_DECRYPTAES", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "OP_VERIFYSIG", "OP_CODESEPARATOR", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "OP_CALL", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "OP_JUMP", "OP_JUMPIF", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", "Invalid Operator", };

enum ScriptOperator {
    OP_NOP = 0x00,
    OP_PUSH = 0x01,
    OP_INSTRUCTION = 0x02,
    OP_FLAG = 0x03,
    OP_BIGPUSH = 0x04,
    OP_FLAGDATA = 0x05,
    OP_TIME = 0x06,
    OP_PICK = 0x07,
    OP_PUT = 0x08,
    OP_NUMEQUAL = 0x10,
    OP_BYTESEQUAL = 0x11,
    OP_SHA512EQUAL = 0x12,
    OP_LENEQUAL = 0x13,
    OP_LESSTHAN = 0x14,
    OP_LESSTHANEQUAL = 0x15,
    OP_GREATERTHAN = 0x16,
    OP_GREATERTHANEQUAL = 0x17,
    OP_NOTEQUAL = 0x18,
    OP_NOTZERO = 0x19,
    OP_GET = 0x1A,
    OP_SET = 0x1B,
    OP_COPY = 0x1C,
    OP_ALLOC = 0x1D,
    OP_THIS = 0x1E,
    OP_ADD = 0x20,
    OP_SUBTRACT = 0x21,
    OP_MULTIPLY = 0x22,
    OP_DIVIDE = 0x23,
    OP_ADDBYTES = 0x24,
    OP_SUBTRACTBYTES = 0x25,
    OP_MULTIPLYBYTES = 0x26,
    OP_DIVIDEBYTES = 0x27,
    OP_NEGATE = 0x28,
    OP_MODULO = 0x29,
    OP_CONVERT8TO32 = 0x2A,
    OP_CONVERT32TO8 = 0x2B,
    OP_CONVERT64TO32 = 0x2C,
    OP_CONVERT32TO64 = 0x2D,
    OP_CONVERTFLOATTO32 = 0x2E,
    OP_CONVERT32TOFLOAT = 0x2F,
    OP_BITNOT = 0x31,
    OP_BITOR = 0x32,
    OP_BITAND = 0x33,
    OP_BITXOR = 0x34,
    OP_APPEND = 0x40,
    OP_LIMIT = 0x41,
    OP_REVERSE = 0x42,
    OP_SPLIT = 0x43,
    OP_COMBINE = 0x44,
    OP_LEN = 0x45,
    OP_NOT = 0x50,
    OP_OR = 0x51,
    OP_AND = 0x52,
    OP_XOR = 0x53,
    OP_INVERTFLOAT = 0x60,
    OP_NEGATEFLOAT = 0x61,
    OP_ADDFLOAT = 0x62,
    OP_SUBTRACTFLOAT = 0x63,
    OP_MULTIPLYFLOAT = 0x64,
    OP_DIVIDEFLOAT = 0x65,
    OP_NULL = 0x81,
    OP_FALSE = 0x82,
    OP_TRUE = 0x83,
    OP_DEPTH = 0x90,
    OP_DROP = 0x91,
    OP_DUP = 0x92,
    OP_SWAP = 0x93,
    OP_CLEAR = 0x94,
    OP_CLONE = 0x95,
    OP_FLIP = 0x96,
    OP_SHIFTUP = 0x97,
    OP_SHIFTDOWN = 0x98,
    OP_SHIFTN = 0x99,
    OP_SHIFTELEMENTSRIGHT = 0x9A,
    OP_SHIFTELEMENTSLEFT = 0x9B,
    OP_DUP2 = 0x9C,
    OP_DUPN = 0x9D,
    OP_DROPN = 0x9E,
    OP_SHIFTNEXCEPT = 0x9F,
    OP_VERIFY = 0xA0,
    OP_RETURN = 0xA1,
    OP_RETURNIF = 0xA2,
    OP_SHA512 = 0xB0,
    OP_ZIP = 0xB1,
    OP_UNZIP = 0xB2,
    OP_ENCRYPTAES = 0xB5,
    OP_DECRYPTAES = 0xB6,
    OP_VERIFYSIG = 0xC0,
    OP_CODESEPARATOR = 0xC1,
    OP_CALL = 0xD0,
    OP_JUMP = 0xF0,
    OP_JUMPIF = 0xF1,
};

#endif //UISCOINSCRIPTVM_SCRIPTOPERATOR_H
