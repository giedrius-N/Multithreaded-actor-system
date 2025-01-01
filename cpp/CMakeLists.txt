cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(actor_system)

include_directories(include)

# CAF - C++ Actor Framework
set(CAF_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cpp_actor_framework")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(actor_system ${SRC_DIR}/main.cpp)

include_directories(
    "${CAF_DIR}/include"
)

target_link_libraries(actor_system
    debug;${CAF_DIR}//lib//Debug//caf_core.lib;optimized;${CAF_DIR}//lib//Release//caf_core.lib
    debug;${CAF_DIR}//lib//Debug//caf_io.lib;optimized;${CAF_DIR}//lib//Release//caf_io.lib
    debug;${CAF_DIR}//lib//Debug//caf_net.lib;optimized;${CAF_DIR}//lib//Release//caf_net.lib
    debug;${CAF_DIR}//lib//Debug//caf_openssl.lib;optimized;${CAF_DIR}//lib//Release//caf_openssl.lib
)
