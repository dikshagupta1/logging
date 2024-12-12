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
│ ├── sample_wrapper.h
│ ├── resampler.h
│ ├── ultrasonic_amp_log.h 
├── src/ # Source files for implementation
│ ├── log_interface_c.cpp
│ ├── sample_wrapper.cpp
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
    ./logging

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
#### For architecture overview and developer notes about the design and decisions read [Developer Overview](DEVELOPER_OVERVIEW.md).

---
## **Future Work**
- Initially I have implemented the structure and logic for Ultraonic Amplitude Log. Future enhancements include adding support for other three logs.






