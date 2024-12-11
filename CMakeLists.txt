
cmake_minimum_required(VERSION 3.15)

# Project Name
project(Logging LANGUAGES CXX)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Include Directory
include_directories(include)

# Add Source Files
set(SOURCES
    src/log_data_wrapper.cpp
    src/log_interface_c.cpp
    src/ultrasonic_amp_log.cpp
    src/resampler.cpp
    src/main.cpp
)

# Add Executable
add_executable(loggind ${SOURCES})

# Add Tests
enable_testing()
add_executable(run_tests tests/resampler_test.cpp)
add_test(NAME ResamplerTests COMMAND run_tests)
