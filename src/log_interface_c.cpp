// log_interface.cpp
#include "log_interface_c.h"
#include "log_interface.h"
#include "ultrasonic_amp_log.h"
#include <cstring>
#include <string.h>

// Factory function to create appropriate LogData objects based on LogType
LogHandle load_log_data_c(const char* file_path, LogType log_type) {
    LogData* log = nullptr;

    switch (log_type) {
        case ULTRASONIC_AMPLITUDE:
            log = new UltrasonicAmplitudeLogData();
            break;
        // Add cases for other log types...
        default:
            return nullptr;
    }

    try {
        log->load(file_path);
    } catch (...) {
        delete log;
        return nullptr;
    }

    return static_cast<LogHandle>(log);
}

int get_log_subset_c(LogHandle log_handle, float start, float end, float* out_subset, int max_size) {
    if (!log_handle || !out_subset) return -1;

    LogData* log = static_cast<LogData*>(log_handle);
    std::vector<float> subset = log->getSubset(start, end);

    if (subset.size() > static_cast<size_t>(max_size)) {
        // Not enough space in out_subset
        return -1;
    }

    std::memcpy(out_subset, subset.data(), subset.size() * sizeof(float));
    return static_cast<int>(subset.size());
}

int get_log_metadata_c(LogHandle log_handle, LogMetadata* metadata) {
    if (!log_handle || !metadata) return -1;

    LogData* log = static_cast<LogData*>(log_handle);
    *metadata = log->getMetadata();

    return 0;
}

void free_log_data_c(LogHandle log_handle) {
    if (!log_handle) return;

    LogData* log = static_cast<LogData*>(log_handle);
    delete log;
}
