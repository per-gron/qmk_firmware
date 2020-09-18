#pragma once

#include <stdint.h>

typedef struct {
    // This is a read-only field. It is 0 initially and is set to 1 when
    // the circular buffer is full. Before this the moving average value
    // will include initial zeroes.
    uint8_t buffer_is_full;
    // Don't access these directly.
    int64_t sum;
    int32_t* values;
    int32_t value_count;
    int16_t next_sample;
} MovingAverage;

// Initialize a MovingAverage struct.
void moving_average_init(MovingAverage* obj, int32_t value_count);

// Get the current moving average value.
int32_t moving_average_get(const MovingAverage* obj);

// Overwrite all values with a given value, forcing the average to be that value.
void moving_average_set(MovingAverage* obj, int32_t value);

// Push a new value into the moving average data structure.
void moving_average_push_value(MovingAverage* obj, int32_t value);
