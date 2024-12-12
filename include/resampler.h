#ifndef RESAMPLER_H
#define RESAMPLER_H

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include "log_types.h"

class Resampler {
public:
    /**
     * Resamples an UltrasonicAmplitudeLog to an evenly gridded form.
     * @param log The original log data to resample.
     * @param start_depth Starting depth of the range of interest.
     * @param end_depth Ending depth of the range of interest.
     * @param depth_points Number of points to resample in the depth dimension.
     * @param azimuth_points Number of points to resample in the azimuth dimension.
     * @return A resampled 2D vector with evenly gridded data.
     */
    static std::vector<std::vector<float>> resampleToDepthGrid(
        const UltrasonicAmplitudeLog& log,
        float start_depth,
        float end_depth,
        size_t depth_points,
        size_t azimuth_points
    );
    
private:
    //static float interpolate(const std::vector<float>& x, const std::vector<float>& y, float xi);
};

#endif // RESAMPLER_H



/*
#include "log_data.h"
#include <vector>

class Resampler {
public:
    std::vector<std::vector<float>> resample(
        const UltrasonicAmplitudeLog& log,
        float start_depth,
        float end_depth,
        size_t depth_points,
        size_t azimuth_points) {
        
        std::vector<std::vector<float>> resampled(depth_points, std::vector<float>(azimuth_points, 0.0f));
        float depth_step = (end_depth - start_depth) / depth_points;
        
        for (size_t i = 0; i < depth_points; ++i) {
            float current_depth = start_depth + i * depth_step;
            size_t row_index = findNearestDepthIndex(log, current_depth);
            for (size_t j = 0; j < azimuth_points; ++j) {
                size_t column_index = (j * log.columns) / azimuth_points;
                resampled[i][j] = log.data[row_index * log.columns + column_index];
            }
        }
        return resampled;
    }

private:
    size_t findNearestDepthIndex(const UltrasonicAmplitudeLog& log, float depth) {
        // Implement logic to find the nearest depth index
        // This is just a placeholder
        return 0;
    }
};
*/