#ifndef LOG_INTERFACE_H
#define LOG_INTERFACE_H

#include "log_types.h"
#include <vector>
#include <string>

/**
 * @brief Interface for loading and accessing log data.
 */
class LogData {
public:
    virtual void load(const std::string& file_path) = 0; // Load log data from a file
    virtual std::vector<float> getSubset(float start_depth, float end_depth) = 0; // Extract a subset of data
    virtual LogMetadata* getMetadata() const = 0; // Retrieve log metadata
    virtual ~LogData() = default;
};

#endif // LOG_INTERFACE_H