#ifndef LOG_INTERFACE_C_H
#define LOG_INTERFACE_C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "log_types.h"
#include <stdint.h>

/**
 * @brief Enum for specifying the type of axis.
 */
typedef enum {
    AXIS_TIME,
    AXIS_DEPTH
} AxisType;

/**
 * @brief Enum for specifying the type of log.
 */
typedef enum {
    ULTRASONIC_AMPLITUDE,
    GAMMA_RAY,
    OPTICAL_IMAGE,
    SONIC_RX
} LogType;

/**
 * @brief Opaque pointer to a LogData object.
 */
typedef void* LogHandle;

/**
 * @brief Load log data from a file.
 *
 * This function reads log data from a file and returns a pointer to the
 * appropriate log structure. The type of log is specified as an input.
 *
 * @param file_path Path to the file containing the log data.
 * @param log_type Type of the log (e.g., ULTRASONIC_AMPLITUDE, GAMMA_RAY).
 * @return Handle to the loaded log data. Cast to the appropriate log type.
 */
LogHandle load_log_data_c(const char* file_path, LogType log_type);

/**
 * @brief Retrieve a subset of log data.
 *
 * This function extracts a subset of the log data within the specified
 * depth or time range. The subset is returned as a new log structure.
 *
 * @param log Handle to the log data structure.
 * @param start Reference starting depth or time.
 * @param end Reference ending depth or time.
 * @param out_subset Pointer to an array where subset data will be stored.
 *                   The caller is responsible for allocating sufficient memory.
 * @param max_size Maximum number of elements that can be written to out_subset.
 * @return Number of elements written to out_subset, or -1 on failure.
 */
int get_log_subset_c(LogHandle log, float start, float end, float* out_subset, int max_size);

/**
 * @brief Retrieve metadata for a log.
 *
 * This function extracts the metadata for the provided log data structure.
 *
 * @param log Handle to the log data structure.
 * @param name Buffer to store the log name.
 * @param name_size Size of the name buffer.
 * @param units Buffer to store the units.
 * @param units_size Size of the units buffer.
 * @param axis Buffer to store the axis type.
 * @param axis_size Size of the axis buffer.
 * @return 0 on success, non-zero on failure.
 */
int get_log_metadata_c(LogHandle log, char* name, uint8_t name_size, char* units, uint8_t units_size, char* axis, uint8_t axis_size);

/**
 * @brief Free memory allocated for log data.
 *
 * This function releases all memory associated with a loaded log data
 * structure, ensuring no memory leaks occur.
 *
 * @param log Handle to the log data structure..
 */
void free_log_data_c(LogHandle log);

#ifdef __cplusplus
}
#endif

#endif // LOG_INTERFACE_C_H

