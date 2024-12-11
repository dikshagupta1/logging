#include <cassert>
#include <iostream>
#include "log_types.h"
#include "resampler.h"

void test_resampler() {
    /*size_t rows = 5;
    size_t columns = 10;
    float azimuth_step = 0.6;
    std::vector<float> data(rows * columns, 1.0f); // Dummy data
    LogMetadata metadata = {"Ultrasonic Log", "arbitrary units", AXIS_DEPTH};

    UltrasonicAmplitudeLog log = {
        data.data(),
        rows,
        columns,
        azimuth_step,
        metadata
    };

    Resampler resampler;
    auto resampled = resampler.resample(log, 0.0f, 5.0f, 3, 5);

    // Validate size
    assert(resampled.size() == 3);
    assert(resampled[0].size() == 5);

    // Validate values
    for (const auto& row : resampled) {
        for (float val : row) {
            assert(val == 1.0f);
        }
    }

    std::cout << "All tests passed!\n";*/
}

int main() {
    test_resampler();
    return 0;
}
