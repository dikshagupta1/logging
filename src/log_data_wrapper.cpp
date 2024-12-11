#include "log_data_wrapper.h"
#include <stdexcept>
#include <cstring>

LogDataWrapper::LogDataWrapper(const std::string& file_path, LogType type) {
    handle = load_log_data(file_path.c_str(), type);
    if (!handle) {
        throw std::runtime_error("Failed to load log data.");
    }
}

LogDataWrapper::~LogDataWrapper() {
    free_log_data_c(handle);
}

std::vector<float> LogDataWrapper::getSubset(float start, float end) {
    // Estimate maximum possible size
    // For example, if each row has 'cols' elements and we have 'rows' rows
    // Here, assuming 1000 elements is sufficient; adjust as needed
    float buffer[1000];
    int num_elements = get_log_subset(handle, start, end, buffer, 1000);
    if (num_elements < 0) {
        throw std::runtime_error("Failed to get log subset.");
    }
    return std::vector<float>(buffer, buffer + num_elements);
}

LogMetadata LogDataWrapper::getMetadata() const {
    char name[50];
    char units[20];
    char axis[20];
    int result = get_log_metadata(handle, name, 50, units, 20, axis, 20);
    if (result != 0) {
        throw std::runtime_error("Failed to get log metadata.");
    }
    LogMetadata metadata;
    metadata.name = std::string(name);
    metadata.units = std::string(units);
    metadata.axis = std::string(axis);
    return metadata;
}
