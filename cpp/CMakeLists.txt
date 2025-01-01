cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(actor_system)

include_directories(include)

set(src_dir "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(actor_system ${src_dir}/main.cpp)
