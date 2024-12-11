#include <iostream>
#include <vector>
#include "log_data.h"
#include "resampler.cpp"

void print_resampled_data(const std::vector<std::vector<float>>& data) {
    for (const auto& row : data) {
        for (float val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    // Example dataset for Ultrasonic Amplitude Log
    size_t rows = 5;
    size_t columns = 10;
    float azimuth_step = 0.6;
    std::vector<float> data(rows * columns, 1.0f); // Dummy data
    Metadata metadata = {"Ultrasonic Log", "arbitrary units", AXIS_DEPTH};

    UltrasonicAmplitudeLog log = {
        data.data(),
        rows,
        columns,
        azimuth_step,
        metadata
    };

    Resampler resampler;
    auto resampled = resampler.resample(log, 0.0f, 5.0f, 3, 5);

    std::cout << "Resampled Data:\n";
    print_resampled_data(resampled);

    // Example 2
    // Load Ultrasonic Amplitude log
    UltrasonicAmplitudeLog* log = (UltrasonicAmplitudeLog*)load_log_data("ultrasonic_log.dat", ULTRASONIC_AMPLITUDE);
    if (!log) {
        printf("Failed to load log data.\n");
        return 1;
    }

    // Get metadata
    Metadata metadata = get_log_metadata(log);
    printf("Log Name: %s, Units: %s\n", metadata.name, metadata.units);

    // Extract a subset
    UltrasonicAmplitudeLog* subset = (UltrasonicAmplitudeLog*)get_log_subset(log, 0.0f, 10.0f);
    if (subset) {
        printf("Subset extracted successfully.\n");
        free_log_data(subset);
    }

    // Free memory
    free_log_data(log);

    //Example 3
    // Simulated log data for testing
    UltrasonicAmplitudeLog log;
    log.name = "Ultrasonic Amplitude Log";
    log.unit = "Amplitude";
    log.depths = new float[5]{0.0f, 0.5f, 1.0f, 1.5f, 2.0f};
    log.depth_count = 5;
    log.azimuth_count = 6; // 0°, 60°, ..., 300°
    log.data = new float[30]{
        1.0f, 1.2f, 1.1f, 1.3f, 1.4f, 1.5f,  // Depth 0.0
        2.0f, 2.1f, 2.2f, 2.3f, 2.4f, 2.5f,  // Depth 0.5
        3.0f, 3.1f, 3.2f, 3.3f, 3.4f, 3.5f,  // Depth 1.0
        4.0f, 4.1f, 4.2f, 4.3f, 4.4f, 4.5f,  // Depth 1.5
        5.0f, 5.1f, 5.2f, 5.3f, 5.4f, 5.5f   // Depth 2.0
    };

    // Resample
    try {
        auto resampled_data = Resampler::resampleToDepthGrid(log, 0.5f, 1.5f, 3, 6);

        // Print results
        for (const auto& row : resampled_data) {
            for (float value : row) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Clean up
    delete[] log.depths;
    delete[] log.data;

    return 0;
}
