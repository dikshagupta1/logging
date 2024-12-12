## Developer Overview

---

### Data Structure
**Where implemented: In log_types.h**

Details:

The data structures (UltrasonicAmplitudeLog, GammaRayLog, OpticalImageLog, SonicRXLog) are C-style structs representing the logs.
Each struct is designed to encapsulate:
- The data/value (data fields as std::vector).
- Metadata (via the Metadata struct) that encapsulates names, units, and axes.
- Additional information specific to each log (e.g., azimuth step, RGB tuples).
The use of std::vector ensures efficient memory mapping and rapid access.
They are extensible for higher dimensionality or additional value types by adding fields or modifying the structure.

1. **Key Features of Log 1: Ultrasonic Amplitude**
- Azimuthal Image Data:
Horizontal axis represents azimuth (angles 0°–360°), evenly spaced (e.g., 0.6° apart).
Values are floating-point numbers for each grid point.
- Depth or Time Referencing:
Each row of the image corresponds to a specific depth or time.
Depth or time serves as the vertical axis reference.
- Evenly Gridded Columns:
Azimuth values are fixed and evenly spaced.

|Log Feature |	Struct Field |	Explanation|
|------------|-------------|-------------|
|Depth/Time Rows|axis_value	|Array of depths/time (one for each row in the log).|
|Floating-Point Amplitude Data|amplitude_data	|2D array of amplitude values.|
|Fixed Azimuth Spacing|	azimuth_spacing|Defines how azimuth points are spaced (e.g. 0.6°)|
|Number of Depths/Time (Rows)|	axis_count|	Total number of rows in the log.|
|Number of Azimuths (Columns)|	azimuth_count|	Total number of azimuth points per row.|
|Timestamp|timestamp|Timestamp at which the time/depth value was taken.|

**Example Usage:**
```
#include "log_types.h"
#include <stdlib.h>

int main() {
    UltrasonicAmplitudeLog log;

    // Define azimuth and depth ranges
    log.axis_count = 100;             // 100 depth points
    log.azimuth_count = 600;           // 600 azimuth points (360° / 0.6°)
    log.azimuth_spacing = 0.6f;        // Azimuth spacing in degrees

    // Allocate memory for depths
    log.axis_value = (float*)malloc(log.axis_count * sizeof(float));
    for (uint32_t i = 0; i < log.axis_count; ++i) {
        log.axis_value[i] = i * 0.5f; // Example depth values (0.0, 0.5, 1.0, ...)
    }

    // Allocate memory for amplitude data (2D array)
    log.amplitude_data = (float**)malloc(log.axis_count * sizeof(float*));
    for (uint32_t i = 0; i < log.axis_count; ++i) {
        log.amplitude_data[i] = (float*)malloc(log.azimuth_count * sizeof(float));
        for (uint32_t j = 0; j < log.azimuth_count; ++j) {
            log.amplitude_data[i][j] = (float)(i + j); // Example amplitude data
        }
    }

    // Clean up
    for (uint32_t i = 0; i < log.axis_count; ++i) {
        free(log.amplitude_data[i]);
    }
    free(log.amplitude_data);
    free(log.axis_value);

    return 0;
}
```

2. **Key Features of Log 2: Gamma Ray log**

- Single Numeric Value:
Each depth (or time) is associated with a single gamma ray activity level.
This means there’s a one-to-one mapping between depth/time and gamma ray activity.
- Floating Point Values:
Gamma ray activity levels are represented as floating-point numbers in API units.
- Plottable as Line Chart:
The data can be plotted as a line chart with depth/time on the vertical axis and activity level on the horizontal axis.
- Depth or Time Reference:
Each data point is referenced either by depth or time, and these references are strictly increasing for time and monotonic for depth.

|Log Feature |	Struct Field |	Explanation|
|------------|-------------|-------------|
|Depth/Time Rows|axis_value	|Array of depth/time values corresponding to each gamma ray activity measurement.|
|Gamma Values|activity_levels	|Array of gamma ray activity levels, one value for each depth.|
|Number of Depths/Time (Rows)|	axis_count|Number of depth (or time) samples.|

3. **Key Features of Log 3: Optical Image log**

- Azimuthal Image Log:
Like Log 1 (Ultrasonic Amplitude), it has azimuthal data, meaning the horizontal axis represents angles around a borehole.
Each row corresponds to a specific depth.
- RGB Tuples:
Instead of floating-point values (as in Log 1), each point in the log is represented by an RGB tuple.
Each RGB channel can be 8 bits (standard) or more (e.g., 10 or 12 bits for high dynamic range).
- Depth Referencing:
Similar to Log 1, each row in the image is referenced to a particular depth.
- Grid Characteristics:
The horizontal axis is evenly spaced azimuthally.
Each grid point is defined by coordinates [depth, azimuth].

|Log Feature |	Struct Field |	Explanation|
|------------|-------------|-------------|
|Depth/Time Rows|axis_value	|Array of depth/time values row.|
|RGB tuples|rgb_values	|A flattened array containing the RGB tuples for the entire log.The array size is depth_count * azimuth_count * 3 (three values per point for R, G, and B channels).|
|Number of Depths/Time (Rows)|	axis_count|Number of depth (or time) samples.|
|Number of Azimuths (Columns)|	azimuth_count|	Total number of azimuth points per row. This is fixed for the entire log and represents the resolution of the horizontal axis.|
|Channel Range|	bits_per_channel|Indicates the number of bits per channel (e.g., 8 for standard RGB, 10 or 12 for high dynamic range images).|

**Example Usage:**
To access an RGB value for a given depth and azimuth:
```
uint32_t index = (depth_idx * azimuth_count + azimuth_idx) * 3;
uint16_t red   = log->rgb_values[index];
uint16_t green = log->rgb_values[index + 1];
uint16_t blue  = log->rgb_values[index + 2];

// Adjust based on bits_per_channel
if (log->bits_per_channel < 16) {
    uint16_t mask = (1 << log->bits_per_channel) - 1;
    red &= mask;
    green &= mask;
    blue &= mask;
}

// Now red, green, and blue are appropriately masked values.
```

Loading Different Bit Depths:
- For 8-bit data:
```
log->rgb_values[index] = source_red * 257;
log->rgb_values[index + 1] = source_green * 257;
log->rgb_values[index + 2] = source_blue * 257;
log->bits_per_channel = 8;

```
- For 10-bit or 12-bit data:
```
log->rgb_values[index] = source_red;
log->rgb_values[index + 1] = source_green;
log->rgb_values[index + 2] = source_blue;
log->bits_per_channel = 10; // Or 12, as applicable

```

4. **Key Features of Log 4: SonicRX log**

- Waveforms Represent Time-Dependent Data:
Each depth or time has an associated waveform.
Waveforms consist of floating-point values representing amplitude over time.
- Variable Number of Points per Waveform:
There is no guarantee that each waveform has the same number of points.
Each waveform may be sampled differently (non-uniform sampling).
- Floating Point Values:
Data values are represented as floating-point numbers.

|Log Feature |	Struct Field |	Explanation|
|------------|-------------|-------------|
|Depth/Time Rows|axis_value	| Depth or time at which each waveform is sampled.|
|Waveforms of Variable Length |waveforms 	|The waveforms member is an array of pointers to individual waveforms. Each pointer points to a dynamically allocated array of floating-point values, representing amplitude. Holds the amplitude values for each waveform.|
|Number of Depths/Time (Rows)|	axis_count|Number of depth (or time) samples.|
|Points |	point_counts | Specifies the number of points in each waveform, accommodating the variable lengths.|
|Points |	times |This is an array of pointers to time arrays. Each pointer corresponds to a waveform and points to a dynamically allocated array of time values. This handles non-uniform sampling.|

**Example Usage:**
Here’s an example of how to populate a SonicRXLog with non-uniform time sampling:
```
#include <stdlib.h>
#include <stdio.h>

int main() {
    SonicRXLog log;
    log.metadata.name = "SonicRX";
    log.metadata.units = "Amplitude";
    log.metadata.axis = "AXIS_DEPTH";

    // Depths
    float depths[] = {100.0, 101.0, 102.0};
    log.axis_va;lue = depths;
    log.axis_count = 3;

    // Allocate memory for waveforms and times
    log.waveforms = malloc(3 * sizeof(float*));
    log.times = malloc(3 * sizeof(float*));
    log.point_counts = malloc(3 * sizeof(uint32_t));

    // Waveform 1
    float waveform1[] = {0.1, 0.3, 0.5};
    float times1[] = {0.0, 0.05, 0.1};
    log.waveforms[0] = waveform1;
    log.times[0] = times1;
    log.point_counts[0] = 3;

    // Waveform 2
    float waveform2[] = {0.2, 0.4};
    float times2[] = {0.0, 0.07};
    log.waveforms[1] = waveform2;
    log.times[1] = times2;
    log.point_counts[1] = 2;

    // Waveform 3
    float waveform3[] = {0.0, 0.1, 0.2, 0.3};
    float times3[] = {0.0, 0.03, 0.08, 0.12};
    log.waveforms[2] = waveform3;
    log.times[2] = times3;
    log.point_counts[2] = 4;

    // Accessing data
    for (uint32_t i = 0; i < log.axis_count; ++i) {
        printf("Depth: %f\n", log.axis_value[i]);
        printf("Waveform (Time vs Amplitude):\n");
        for (uint32_t j = 0; j < log.point_counts[i]; ++j) {
            printf("Time: %f, Amplitude: %f\n", log.times[i][j], log.waveforms[i][j]);
        }
        printf("\n");
    }

    // Free dynamically allocated memory if necessary
    // In this example, the arrays are static, so no freeing is needed for waveforms and times.
    free(log.waveforms);
    free(log.times);
    free(log.point_counts);

    return 0;
}
```

---

### Interface
1. **Base Class Interface**
    
    **Where implemented: In log_interface.h**

    Details:

    The LogData class serves as the base interface for all log types.
    It defines virtual methods:
    - load: To load data from a file.
    - getSubset: To access subsets of the data based on depth or time ranges.
    - getMetadata: To retrieve metadata for encapsulating units, names, and axes.
    Being an abstract class, LogData ensures extensibility by allowing specialized implementations for each log type.
    Specific implementations (e.g., UltrasonicAmplitudeLogData) will inherit and implement these methods.

    High-level abstraction:
    - The derived classes (e.g., UltrasonicAmplitudeLogData, GammaRayLogData) provide specific functionality for different log types.
    - For example, they can define logic for rapid seeking/searching through data.

2. **C FFI Interface**
    
    **Where implemented: In log_interface_c.h**

    Details:

    - The header `log_interface_c.h` exposes the C functions for the FFI. It uses opaque pointers (void*) to hide the actual C++ class implementations.
    - The C functions declared in `log_interface_c.h` are implemented in `log_interface_c.cpp`. These functions internally manage the C++ objects.

    **Functions**
    1. load_log_data_c:
        
        Dynamically loads a specific type of log data from a file.
        Takes the file path and log type as input.
        Returns a generic void* pointer that can be cast to the appropriate log type (e.g., UltrasonicAmplitudeLog*).

    2. get_log_subset_c:

        Extracts a subset of the log data within a specified range (depth or time).
        Useful for analyzing or visualizing specific sections of the dataset.

    3. get_log_metadata_c:

        Provides access to the metadata of the log, such as name, units, and axis type.
        Helps in understanding the context and structure of the log.

    4. free_log_data_c:

        Frees all dynamically allocated memory associated with the log data.
        Ensures efficient memory management and avoids leaks.

    - The void* return type in load_log_data_c and get_log_subset_c allows flexibility for multiple log types. Cast it to the appropriate type based on LogType.
    - The interface is lightweight, providing only essential functionality for interacting with log data while keeping the implementation hidden.

3. **High-Level Abstraction (Optional)**
    
    **Where implemented: In sample_wrapper.h**

    Details:

    - While the C FFI exposes a C-compatible interface, we might also want to provide a higher-level abstraction in C++, Rust, Python or another language. 
    This is optional but can enhance usability.
---

### Resampling
**Where implemented: In resampler.h**

Details:

The Resampler class:

- Accepts a dataset, depth and azimuth ranges, and the number of points for resampling in its resample method.
- Use interpolation and other resampling techniques to generate evenly gridded data.
- Ensures the output has floating-point values suitable for visualization (leaving color mapping to later stages).
- Prioritizes the most recent data by maintaining higher Z-order for the latest samples when there are duplicates for the same depth.
- Ensures resampling to a reasonable number of points that can be displayed on a modern monitor.

The implementation (to be added later) will:
- Handle input validation (e.g., ensuring proper depth/azimuth ranges).
- Error handling mechanism

---

### Problem Statement and Code Translation
1. **Task: Resampling the Data**

    Requirement: 
    Resample a dataset to evenly gridded data for display against a depth axis.
    Code Implementation:
    The resampleToDepthGrid function takes the UltrasonicAmplitudeLog as input, which contains unevenly gridded depth data.
    The function generates an evenly spaced grid for depth (target_depths) and azimuth (azimuth_points).
    Using interpolation, it fills the resampled grid by mapping the original data points to the new evenly spaced grid.
    ```
        static std::vector<std::vector<float>> resampleToDepthGrid(
        const UltrasonicAmplitudeLog& log,
        float start_depth,
        float end_depth,
        size_t depth_points,
        size_t azimuth_points
        );
    ```

2. **Task: Input Parameters**

    Requirement: 
    Inputs include the dataset, depth and azimuth range, and grid size for depth and azimuth.
    Code Implementation:
    Inputs to the resampleToDepthGrid function:
    - log: The original dataset (UltrasonicAmplitudeLog).
    - start_depth and end_depth: Define the range of depths to resample.
    - depth_points and azimuth_points: Specify the number of points to resample in the depth and azimuth dimensions.

3. **Task: Evenly Gridded Output**

    Requirement: 
    The output should be evenly gridded and suitable for display.
    Code Implementation:
    - The target_depths array defines evenly spaced depth points between start_depth and end_depth using a calculated depth_step:
    ```
        float depth_step = (end_depth - start_depth) / (resample_depth_count - 1);
        for (size_t i = 0; i < resample_depth_count; ++i) {
            float target_depth = start_depth + i * depth_step;
            ...
        }
    ```
    - Azimuth is divided into evenly spaced points
    ```
        float azimuth_step = 360.0f / resample_azimuth_count;
    ```

4. **Task: Floating-Point Values**

    Requirement: 
    The resampled output consists of floating-point values.
    Code Implementation:
    The resampled_data structure is a std::vector<std::vector<float>>, representing the resampled grid as floating-point values:
    ```
    std::vector<std::vector<float>> resampled_data(resample_depth_count,std::vector<float>(resample_azimuth_count, 0.0f));
    ```

5. **Task: Prioritize Recently Acquired Data**

    Requirement: 
    When there are multiple samples for the same depth, prioritize the most recently acquired data.

6. **Task: Interpolation**

    Requirement: 
    Interpolate values between unevenly spaced original depths.
    Code Implementation:
    The interpolate function performs bilinear interpolation between 4 surrounding points in the depth array to compute values at the target depth:
    ```
    float v11 = log.amplitude_data[latest_depth_index][azimuth_idx1];
    float v12 = log.amplitude_data[latest_depth_index][azimuth_idx2];
    float v21 = log.amplitude_data[depth_idx2][azimuth_idx1];
    float v22 = log.amplitude_data[depth_idx2][azimuth_idx2];
    float interpolated_value =(1 - depth_weight) * ((1 - azimuth_weight) * v11 + azimuth_weight * v12) + depth_weight * ((1 - azimuth_weight) * v21 + azimuth_weight * v22);
    ```

7. **Task: Display-Ready Data**
    Requirement: 
    Resample data to a reasonable number of points for modern monitors.
    Code Implementation:
    The resample_depth_count and resample_azimuth_count parameters allow control over the resolution of the output grid, making it compatible with display requirements.
    The MAX_POINTS number is maintained to limit the resampling grid.
    ```
    if (total_points > MAX_POINTS) {
            // Try adjusting resample_depth_count first
            float ratio = static_cast<float>(total_points) / MAX_POINTS;

            // Adjust resample_depth_count based on the ratio, but ensure it doesn't go below 1
            resample_depth_count = std::max<size_t>(1, static_cast<size_t>(resample_depth_count / ratio));

            // Recalculate the total points after adjusting resample_depth_count
            total_points = resample_depth_count * resample_azimuth_count;

            // If the total points still exceed MAX_POINTS, adjust resample_azimuth_count
            if (total_points > MAX_POINTS) {
                ratio = static_cast<float>(total_points) / MAX_POINTS;
                resample_azimuth_count = std::max<size_t>(1, static_cast<size_t>(resample_azimuth_count / ratio));
            }
        }
    ```

8. **Test Case Verification**

    Requirement: Verify resampling functionality with appropriate test data.
    Code Implementation:
    The main.cpp file provides a test case where:
    - log is populated with synthetic depth and azimuth data.
    - Resampling is performed for a depth range.
    - The output is printed to verify correctness.
    In future, the resampler_test.cpp will include google/catch2 style test cases.
    
---

### Memory Management Considerations
- Allocation: Log data objects are allocated dynamically (using new in C++). It’s crucial to provide a corresponding free_log_data_c function to release memory and prevent leaks.

- Ownership: Once a LogHandle is obtained via load_log_data, the caller is responsible for eventually calling free_log_data_c to release the resources.

- Error Handling: Functions should handle errors gracefully, returning NULL or error codes as appropriate. Implement proper exception handling in C++ and ensure that exceptions do not cross the C boundary.











