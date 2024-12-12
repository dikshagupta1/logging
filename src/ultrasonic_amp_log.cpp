#include "ultrasonic_amp_log.h"
#include <fstream>
#include <stdexcept>

void UltrasonicAmplitudeLogData::load(const std::string& file_path) {
    // Implement file loading logic
}

std::vector<float> UltrasonicAmplitudeLogData::getSubset(float start_depth, float end_depth) {
    std::vector<float> subset;

    // Find corresponding row indices for start_depth and end_depth and pupulate subset
    return subset;
}

LogMetadata UltrasonicAmplitudeLogData::getMetadata() const {
    return log.metadata;
}
