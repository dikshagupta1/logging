#include "ultrasonic_amp_log.h"
#include <fstream>
#include <stdexcept>

void UltrasonicAmplitudeLog::load(const std::string& file_path) {
    // Implement file loading logic
    // For demonstration, we'll mock the data
    log.rows = 100;
    log.cols = 360;
    log.azimuth_step = 1.0f; // 1 degree step
    log.metadata = {"UltrasonicAmplitudeLog", "arbitrary units", "depth"};

    log.data.resize(log.rows, std::vector<float>(log.cols, 0.0f));

    // Mock data: Fill with incremental values
    for (size_t i = 0; i < log.rows; ++i) {
        for (size_t j = 0; j < log.cols; ++j) {
            log.data[i][j] = static_cast<float>(i * log.cols + j);
        }
    }

    // In a real implementation, read from file_path and populate log.data accordingly
}

std::vector<float> UltrasonicAmplitudeLog::getSubset(float start_depth, float end_depth) {
    std::vector<float> subset;

    // Find corresponding row indices for start_depth and end_depth
    size_t start_row = static_cast<size_t>(start_depth); // Simplified
    size_t end_row = static_cast<size_t>(end_depth);     // Simplified

    if (start_row >= log.rows) start_row = log.rows - 1;
    if (end_row >= log.rows) end_row = log.rows - 1;

    for (size_t i = start_row; i <= end_row; ++i) {
        for (size_t j = 0; j < log.cols; ++j) {
            subset.push_back(log.data[i][j]);
        }
    }

    return subset;
}

LogMetadata UltrasonicAmplitudeLogData::getMetadata() const {
    return log.metadata;
}
