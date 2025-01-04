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

## Test Datasets

This project uses the following test datasets to validate the functionality of the filtering system. The system consists of two filters:

1. **C++ Worker Actors Filter**: The first filter, implemented in C++.
2. **Python Worker Process Filter**: The second filter, implemented in Python.

For an item to appear in `results.txt`, it must pass **both** filters.

### Datasets Description

- **Dataset 1**:  
  All items pass both the **C++ Worker Actors Filter** and the **Python Worker Process Filter**.  
  **Result**: All items are included in `results.txt`.

- **Dataset 2**:  
  None of the items pass the **C++ Worker Actors Filter**, but some pass the **Python Worker Process Filter**.  
  **Result**: No items are included in `results.txt`.

- **Dataset 3**:  
  None of the items pass the **Python Worker Process Filter**, but some pass the **C++ Worker Actors Filter**.  
  **Result**: No items are included in `results.txt`.

- **Dataset 4**:  
  Some items pass the **C++ Worker Actors Filter**, and some pass the **Python Worker Process Filter**.  
  **Result**: Only the items that pass **both filters** are included in `results.txt`.
