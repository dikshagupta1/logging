#ifndef LOG_TYPES_H
#define LOG_TYPES_H

#include <stdint.h>


// Struct for metadata
typedef struct {
    char name[256];  // Name of the log
    char units[32]; // Units of measurement (e.g. "meters", "seconds")
    char axis[32];  // Axis description (e.g. "depth", "time")
} LogMetadata;

// Struct for Ultrasonic Amplitude Log
// Note: the columns and azimuth_step is fixed as per the problem statement. 
// Design approach is to make them variable here an set them to the fixed value when the C FFI are called.
typedef struct {
    float** amplitude_data;    // 2D array of floating-point values for amplitude
    float* axis_value;         // Pointer to array of depths or time values
    size_t axis_count;         // Number of depth/time entries a.k.a No. of rows
    size_t azimuth_count;      // Number of azimuth points a.k.a Number of columns. (Column value is fixed as per the problem statement)
    float azimuth_step;        // Azimuth step in degrees (For our problem statement the step is fixed to 0.6)
    LogMetadata metadata;
} UltrasonicAmplitudeLog;

// Struct for Gamma Ray Log
// Note: As per the problem statement the unit for gamma ray is API which will be set in the metadata and passed.
typedef struct {
    float* activity_levels;     // Pointer to gamma ray activity levels
    float* axis_value;          // Depths/time corresponding to activity levels
    size_t axis_count;          // Number of data points a.k.a No. of rows
    Metadata metadata;
} GammaRayLog;

// Struct for Optical Image Log
typedef struct {
    uint16_t* rgb_values;       // Flattened array of RGB tuples (16 bits per channel)
    float* axis_value;          // Pointer to array of depths or time values
    size_t axis_count;          // Number of depth/time samples a.k.a No. of rows
    size_t azimuth_count;       // Number of azimuthal columns a.k.a Number of columns
    uint8_t bits_per_channel;   // Number of bits per channel (e.g. 8, 10, 12)
    Metadata metadata;
} OpticalImageLog;

// Struct for Sonic RX Log
// Note: As per the problem statement, the axis here is time which will be set in the metadata.
typedef struct {
    float** waveforms;          // Array of pointers to waveform data (variable length)
    float** times;              // Array of pointers to time arrays (variable length)
    uint32_t* point_counts;     // Number of points in each waveform
    float* axis_value;          // Array of depth/time values
    size_t axis_count;          // Number of depth/time samples a.k.a No. of rows
    Metadata metadata;
} SonicRXLog;

#endif // LOG_TYPES_H
