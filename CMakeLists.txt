cmake_minimum_required(VERSION 3.14)
project(UISCoinScriptVM)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
set(CMAKE_CXX_STANDARD 14)

add_executable(UISCoinScriptVM main.cpp ScriptExecution.cpp ScriptExecution.h ScriptOperator.h BytesUtil.cpp BytesUtil.h crypto/sha512.cpp crypto/sha512.h)