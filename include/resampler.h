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