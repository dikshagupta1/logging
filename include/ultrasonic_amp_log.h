#ifndef ULTRASONIC_AMP_LOG_DATA_H
#define ULTRASONIC_AMP_LOG_DATA_H

#include "log_interface.h"

/**
 * @brief Derived class for ultrasonic amplitude log.
 */
class UltrasonicAmplitudeLogData : public LogData {
private:
    UltrasonicAmplitudeLog log;

public:
    void load(const std::string& file_path) override;
    std::vector<float> getSubset(float start_depth, float end_depth) override;
    LogMetadata* getMetadata() const override;
};

#endif // ULTRASONIC_AMP_LOG_DATA_H
