#include "log_data_wrapper.h"
#include <stdexcept>
#include <cstring>
#include <stdint.h>

LogDataWrapper::LogDataWrapper(const std::string& file_path, LogType type) {
    handle = load_log_data_c(file_path.c_str(), type);
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
    int num_elements = get_log_subset_c(handle, start, end, buffer, 1000);
    if (num_elements < 0) {
        throw std::runtime_error("Failed to get log subset.");
    }
    return std::vector<float>(buffer, buffer + num_elements);
}

LogMetadata LogDataWrapper::getMetadata() const {
    // TODO: implementation
    
    LogMetadata *metadata = nil;
    return &metadata;
}
