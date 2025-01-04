cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(actor_system)

include_directories(include)

# CAF - C++ Actor Framework
set(CAF_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cpp_actor_framework")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(actor_system 
    ${SRC_DIR}/Main.cpp
    ${INCL_DIR}/CityTypes.hpp
    ${SRC_DIR}/Utils.cpp
    ${INCL_DIR}/Utils.hpp
    ${SRC_DIR}/SocketUtils.cpp
    ${INCL_DIR}/SocketUtils.hpp
    ${SRC_DIR}/MainActor.cpp
    ${INCL_DIR}/MainActor.hpp
    ${SRC_DIR}/Worker.cpp
    ${INCL_DIR}/Worker.hpp
    ${SRC_DIR}/ResultAccumulator.cpp
    ${INCL_DIR}/ResultAccumulator.hpp
    ${SRC_DIR}/Sender.cpp
    ${INCL_DIR}/Sender.hpp
    ${SRC_DIR}/Getter.cpp
    ${INCL_DIR}/Getter.hpp
    ${SRC_DIR}/Printer.cpp
    ${INCL_DIR}/Printer.hpp
    ${INCL_DIR}/AtomConfig.hpp
)

include_directories(
    "${CAF_DIR}/include"
)

target_link_libraries(actor_system
    debug;${CAF_DIR}//lib//Debug//caf_core.lib;optimized;${CAF_DIR}//lib//Release//caf_core.lib
    debug;${CAF_DIR}//lib//Debug//caf_io.lib;optimized;${CAF_DIR}//lib//Release//caf_io.lib
    debug;${CAF_DIR}//lib//Debug//caf_net.lib;optimized;${CAF_DIR}//lib//Release//caf_net.lib
    debug;${CAF_DIR}//lib//Debug//caf_openssl.lib;optimized;${CAF_DIR}//lib//Release//caf_openssl.lib
    ws2_32
)
