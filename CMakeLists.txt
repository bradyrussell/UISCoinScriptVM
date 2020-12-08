cmake_minimum_required(VERSION 3.14)
project(UISCoinScriptVM)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
set(CMAKE_CXX_STANDARD 14)

link_directories(lib)

add_executable(UISCoinScriptVM main.cpp ScriptExecution.cpp ScriptExecution.h ScriptOperator.h BytesUtil.cpp BytesUtil.h crypto/sha512.cpp crypto/sha512.h zlib/zlib_wrapper.cpp zlib/zlib_wrapper.h)

target_include_directories(UISCoinScriptVM PRIVATE C:\\Users\\Admin\\Desktop\\zlib-1.2.11)
target_link_libraries(UISCoinScriptVM libzlibstatic.a)

