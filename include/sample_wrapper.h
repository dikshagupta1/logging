#ifndef SAMPLE_WRAPPER_H
#define SAMPLE_WRAPPER_H

#include "log_interface_c.h"
#include <string>
#include <vector>

class LogDataWrapper {
private:
    LogHandle handle;

public:
    LogDataWrapper(const std::string& file_path, LogType type);
    ~LogDataWrapper();

    std::vector<float> getSubset(float start, float end);
    LogMetadata& getMetadata();
};

#endif // SAMPE_WRAPPER_H
