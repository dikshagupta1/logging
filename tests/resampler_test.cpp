#include <iostream>
#include <vector>
#include "log_types.h"
#include "resampler.h"
#include <cstring>

// Function to create a sample UltrasonicAmplitudeLog for testing
UltrasonicAmplitudeLog createSampleLog() {
    UltrasonicAmplitudeLog log;

    // Metadata
    strncpy(log.metadata.name, "ULTRASONIC_AMPLITUDE", sizeof(log.metadata.name));
    strncpy(log.metadata.units, "meters", sizeof(log.metadata.units));
    strncpy(log.metadata.axis, "AXIS_DEPTH", sizeof(log.metadata.axis));

    // Define sample depths and timestamps
    log.axis_count = 5;
    log.axis_value = new float[log.axis_count]{100.0, 200.0, 300.0, 400.0, 500.0};
    log.timestamps = new double[log.axis_count]{1.0, 2.0, 3.0, 4.0, 4.0};

    // Define sample azimuth points
    log.azimuth_count = 6;
    log.azimuth_step = 60.0f; // Azimuth steps of 60 degrees (6 points from 0° to 360°)

    // Create sample amplitude data (5 rows x 6 columns)
    log.amplitude_data = new float*[log.axis_count];
    for (size_t i = 0; i < log.axis_count; ++i) {
        log.amplitude_data[i] = new float[log.azimuth_count];
        for (size_t j = 0; j < log.azimuth_count; ++j) {
            log.amplitude_data[i][j] = static_cast<float>(i * 10 + j); // Example values
            std::cout << log.amplitude_data[i][j] << " ";
        }
        std::cout << "\n";
    }
    return log;
}

// Function to clean up dynamically allocated memory in UltrasonicAmplitudeLog
void cleanupLog(UltrasonicAmplitudeLog& log) {
    delete[] log.axis_value;
    for (size_t i = 0; i < log.axis_count; ++i) {
        delete[] log.amplitude_data[i];
    }
    delete[] log.amplitude_data;
}

int main() {
    // Create a sample UltrasonicAmplitudeLog
    UltrasonicAmplitudeLog log = createSampleLog();

    // Define resampling parameters 
    float depth_min = 150.0f;
    float depth_max = 450.0f;
    size_t resample_depth_points = 4;  // Resample to 4 depths
    size_t resample_azimuth_points = 8; // Resample to 8 azimuth points

    // Create Resampler obj
    Resampler resample;

    // Call the resample function
    std::vector<std::vector<float>> resampled_data = resample.resampleToDepthGrid(
        log, depth_min, depth_max, resample_depth_points, resample_azimuth_points);

    // Print the resampled data
    std::cout << "Resampled Data:\n";
    for (size_t i = 0; i < resampled_data.size(); ++i) {
        for (size_t j = 0; j < resampled_data[i].size(); ++j) {
            std::cout << resampled_data[i][j] << " ";
        }
        std::cout << "\n";
    }

    // Clean up allocated memory
    cleanupLog(log);

    return 0;
}