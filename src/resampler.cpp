#include "resampler.h"
#include <cmath>
#include <vector>
#include "log_types.h"
#include <iostream>

// Limit to a reasonable number of points (e.g., a 1080p screen resolution)
#define MAX_POINTS 1920 * 1080  // 1080p resolution, can be adjusted

std::vector<std::vector<float>> Resampler::resampleToDepthGrid(const UltrasonicAmplitudeLog& log,
                                            float start_depth,
                                            float end_depth,
                                            size_t resample_depth_count,
                                            size_t resample_azimuth_count) {
    
    // Ensure the resampling grid does not exceed MAX_POINTS
    size_t total_points = resample_depth_count * resample_azimuth_count;

    // If the total points exceed MAX_POINTS, adjust resample_depth_count or resample_azimuth_count
    if (total_points > MAX_POINTS) {
        // Try adjusting resample_depth_count first
        float ratio = static_cast<float>(total_points) / MAX_POINTS;

        // Adjust resample_depth_count based on the ratio, but ensure it doesn't go below 1
        resample_depth_count = std::max<size_t>(1, static_cast<size_t>(resample_depth_count / ratio));

        // Recalculate the total points after adjusting resample_depth_count
        total_points = resample_depth_count * resample_azimuth_count;

        // If the total points still exceed MAX_POINTS, adjust resample_azimuth_count
        if (total_points > MAX_POINTS) {
            ratio = static_cast<float>(total_points) / MAX_POINTS;
            resample_azimuth_count = std::max<size_t>(1, static_cast<size_t>(resample_azimuth_count / ratio));
        }
    }
    std::cout << "Resampling with " << resample_depth_count << " depth points and "
              << resample_azimuth_count << " azimuth points." << std::endl;
    
    // Create the resampled data grid
    std::vector<std::vector<float>> resampled_data(resample_depth_count,
                                                    std::vector<float>(resample_azimuth_count, 0.0f));

    // Calculate the depth and azimuth steps for the resampled grid
    float depth_step = (end_depth - start_depth) / (resample_depth_count - 1);
    float azimuth_step = 360.0f / resample_azimuth_count;

    for (size_t i = 0; i < resample_depth_count; ++i) {
        float target_depth = start_depth + i * depth_step;

        // Find the indices and weights for depth interpolation
        size_t depth_idx1 = 0, depth_idx2 = 0;
        float depth_weight = 0.0f;

        if (target_depth <= log.axis_value[0]) {
            depth_idx1 = depth_idx2 = 0;
            depth_weight = 0.0f;
        } else if (target_depth >= log.axis_value[log.axis_count - 1]) {
            depth_idx1 = depth_idx2 = log.axis_count - 1;
            depth_weight = 0.0f;
        } else {
            for (size_t d = 0; d < log.axis_count - 1; ++d) {
                if (log.axis_value[d] <= target_depth && target_depth < log.axis_value[d + 1]) {
                    depth_idx1 = d;
                    depth_idx2 = d + 1;
                    depth_weight = (target_depth - log.axis_value[d]) /
                                   (log.axis_value[d + 1] - log.axis_value[d]);
                    break;
                }
            }
        }
         for (size_t j = 0; j < resample_azimuth_count; ++j) {
            float target_azimuth = j * azimuth_step;

            // Find the indices and weights for azimuth interpolation
            size_t azimuth_idx1 = static_cast<size_t>(target_azimuth / log.azimuth_step);
            size_t azimuth_idx2 = (azimuth_idx1 + 1) % log.azimuth_count;

            float azimuth_weight = 0.0f;
            if (azimuth_idx2 > azimuth_idx1) {
                azimuth_weight = (target_azimuth - azimuth_idx1 * log.azimuth_step) /
                                 log.azimuth_step;
            } else {
                //azimuth_weight = 1.0f;
                // Wrap-around interpolation for last-to-first azimuth indices
                azimuth_weight = (target_azimuth - azimuth_idx1 * log.azimuth_step) /
                                 (log.azimuth_step - 360.0f );

            }

            // Prioritize the most recent data by comparing timestamps
            size_t latest_depth_index = depth_idx1;
            double latest_timestamp = log.timestamps[depth_idx1]; // Start with the first depth index's timestamp

            // Check if there's another depth sample with the same depth and prioritize the most recent
            for (size_t k = depth_idx1; k <= depth_idx2; ++k) {
                if (log.axis_value[k] == target_depth) {
                    if (log.timestamps[k] > latest_timestamp) {
                        latest_timestamp = log.timestamps[k];
                        latest_depth_index = k;
                    }
                }
            }

            // Bilinear interpolation: compute value from 4 surrounding points, prioritizing the most recent data
            float v11 = log.amplitude_data[latest_depth_index][azimuth_idx1];
            float v12 = log.amplitude_data[latest_depth_index][azimuth_idx2];
            float v21 = log.amplitude_data[depth_idx2][azimuth_idx1];
            float v22 = log.amplitude_data[depth_idx2][azimuth_idx2];

            float interpolated_value =
                (1 - depth_weight) * ((1 - azimuth_weight) * v11 + azimuth_weight * v12) +
                depth_weight * ((1 - azimuth_weight) * v21 + azimuth_weight * v22);

            resampled_data[i][j] = interpolated_value;
        }
    }

    return resampled_data;
}