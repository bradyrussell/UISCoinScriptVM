cmake_minimum_required(VERSION 3.14)
project(UISCoinScriptVM)

set(CMAKE_CXX_STANDARD 14)

add_executable(UISCoinScriptVM main.cpp ScriptExecution.cpp ScriptExecution.h ScriptOperator.h BytesUtil.cpp BytesUtil.h)