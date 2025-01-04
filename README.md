# Multithreaded-actor-system
A cross-language project implementing a multithreaded actor-based system for distributed data processing. The system features efficient communication between C++ actors (using an Actor Framework) and Python processes (using multiprocessing).

## Building the C++

To build the C++ code, follow these steps:

1. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

2. Configure the project using CMake:
   ```bash
   cmake -G "Visual Studio 17 2022" -A x64 ..
   ```
   You can use any compiler that supports C++ 17

3. Build the project in Debug mode:
   ```bash
   cmake --build . --config Debug
   ```

4. Build the project in Release mode:
   ```bash
   cmake --build . --config Release
   ```

## TODOs:

 - [x] C++ main -> C++ worker -> C++ results accumulator
 - [x] C++ main -> C++ sender -> network (sockets) -> Python getter -> Python worker -> Python sender -> network (sockets) -> C++ getter -> C++ results accumulator -> C++ printer