#include "resampler.h"

/*std::vector<std::vector<float>> Resampler::resampleToDepthGrid(
    const UltrasonicAmplitudeLog& log,
    float start_depth,
    float end_depth,
    size_t depth_points,
    size_t azimuth_points
) {
    if (start_depth >= end_depth || depth_points == 0 || azimuth_points == 0) {
        throw std::invalid_argument("Invalid input parameters for resampling.");
    }

    // Generate evenly spaced depths
    std::vector<float> target_depths(depth_points);
    float depth_step = (end_depth - start_depth) / (depth_points - 1);
    for (size_t i = 0; i < depth_points; ++i) {
        target_depths[i] = start_depth + i * depth_step;
    }

    // Prepare the output grid
    std::vector<std::vector<float>> resampled_data(depth_points, std::vector<float>(azimuth_points, 0.0f));

    // Loop through each depth and azimuth for interpolation
    for (size_t i = 0; i < depth_points; ++i) {
        float target_depth = target_depths[i];

        // Find the nearest data point in the original log
        auto it = std::lower_bound(log.axis_value, log.axis_value + log.axis_count, target_depth);
        size_t idx = std::distance(log.axis_value, it);

        if (idx == 0 || idx == log.axis_count) {
            continue; // Skip if out of bounds
        }

        // Interpolate for each azimuth
        for (size_t j = 0; j < azimuth_points; ++j) {
            float azimuth = j * (360.0f / azimuth_points); // Azimuth evenly spaced
            resampled_data[i][j] = log.amplitude_data[idx * log.azimuth_count + j];
        }
    }

    return resampled_data;
}*/

std::vector<std::vector<float>> resampleLog(
    const UltrasonicAmplitudeLog& log,
    float depth_min, float depth_max,
    size_t resample_depth_points, size_t resample_azimuth_points) {
    
    std::vector<std::vector<float>> resampled_data(
        resample_depth_points, std::vector<float>(resample_azimuth_points, 0.0f));

    float depth_step = (depth_max - depth_min) / resample_depth_points;
    float azimuth_step = 360.0f / resample_azimuth_points;

    for (size_t i = 0; i < resample_depth_points; ++i) {
        float depth = depth_min + i * depth_step;

        // Find the closest depth index in the original data
        size_t idx = 0;
        for (size_t k = 1; k < log.axis_count; ++k) {
            if (std::abs(log.axis_value[k] - depth) < std::abs(log.axis_value[idx] - depth)) {
                idx = k;
            }
        }

        for (size_t j = 0; j < resample_azimuth_points; ++j) {
            // Calculate azimuth based on the resample step
            float azimuth = j * azimuth_step;

            // Map azimuth to original data index
            size_t azimuth_idx = static_cast<size_t>(azimuth / log.azimuth_step) % log.azimuth_count;

            // Access the amplitude_data using 2D indexing
            resampled_data[i][j] = log.amplitude_data[idx][azimuth_idx];
        }
    }

    return resampled_data;
}


float Resampler::interpolate(const std::vector<float>& x, const std::vector<float>& y, float xi) {
    auto it = std::lower_bound(x.begin(), x.end(), xi);
    size_t idx = std::distance(x.begin(), it);

    if (idx == 0) {
        return y[0];
    } else if (idx == x.size()) {
        return y.back();
    }

    float x1 = x[idx - 1];
    float x2 = x[idx];
    float y1 = y[idx - 1];
    float y2 = y[idx];

    return y1 + (xi - x1) * (y2 - y1) / (x2 - x1);
}
