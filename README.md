# Logging Data Exercise

This project provides data structures, interfaces, and a resampling implementation for various types of logging datasets used in fields such as geophysics and well logging.

---

## **Project Structure**
```
/logging 
├── include/ # Header files for data structures and interfaces 
│ ├── log_types.h 
│ ├── log_interface.h
│ ├── log_interface_c.h
│ ├── log_data_wrapper.h
│ ├── resampler.h
│ ├── ultrasonic_amp_log.h 
├── src/ # Source files for implementation
│ ├── log_interface_c.cpp
│ ├── log_data_wrapper.cpp
│ ├── resampler.cpp 
│ ├── main.cpp 
│ ├── ultrasonic_amp_log.cpp
├── tests/ # Unit tests 
│ ├── resampler_test.cpp 
├── CMakeLists.txt # Build system 
├── README.md # Project documentation
```

---

## **Building the Project**

### Prerequisites
1. A C++17-compatible compiler (e.g., GCC, Clang, or MSVC).
2. CMake version 3.15 or later.

### Build Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/dikshagupta1/logging.git
2. Create a build directory and run CMake:
    ```bash
    mkdir build && cd build
    cmake ..
    make
3. Run the program:
    ```bash
    ./log_exercise

### Testing
Tests are located in the tests/ folder and built with the main project. Run tests using:
```bash
./run_tests
```

---

## **Code Overview**
### Data Structures
Defined in log_data.h, these structs represent various types of logs:

- UltrasonicAmplitudeLog: Stores amplitude values as a grid of floating-point numbers.
- GammaRayLog: Represents gamma ray activity levels.
- OpticalImageLog: Contains RGB data for azimuthal images.
- SonicRXLog: Handles waveform data for sonic logging.

### Resampling
Implemented in resampler.cpp, the Resampler class:

- Takes a dataset (e.g., UltrasonicAmplitudeLog).
- Resamples it to an evenly spaced grid for visualization against a depth axis.

### Developer Notes
**Functions**
1. load_log_data:
    
    Dynamically loads a specific type of log data from a file.
    Takes the file path and log type as input.
    Returns a generic void* pointer that can be cast to the appropriate log type (e.g., UltrasonicAmplitudeLog*).

2. get_log_subset:

    Extracts a subset of the log data within a specified range (depth or time).
    Useful for analyzing or visualizing specific sections of the dataset.

3. get_log_metadata:

    Provides access to the metadata of the log, such as name, units, and axis type.
    Helps in understanding the context and structure of the log.

4. free_log_data:

    Frees all dynamically allocated memory associated with the log data.
    Ensures efficient memory management and avoids leaks.

**Notes**
- The void* return type in load_log_data and get_log_subset allows flexibility for multiple log types. Cast it to the appropriate type based on LogType.
- The interface is lightweight, providing only essential functionality for interacting with log data while keeping the implementation hidden.

---
## **Future Work**
- Add support for dynamic range in OpticalImageLog.
- Extend interfaces to support data saving.






