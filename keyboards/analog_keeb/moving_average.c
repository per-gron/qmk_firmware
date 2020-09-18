#include "moving_average.h"

#include <stdlib.h>
#include <string.h>

void moving_average_init(MovingAverage* obj, int32_t value_count) {
    memset(obj, 0, sizeof(MovingAverage));
    obj->values = calloc(value_count, sizeof(*obj->values));
    obj->value_count = value_count;
}

int32_t moving_average_get(const MovingAverage* obj) {
    return obj->sum / ((int64_t)obj->value_count);
}

void moving_average_set(MovingAverage* obj, int32_t value) {
    for (int32_t i = 0; i < obj->value_count; i++) {
        obj->values[i] = value;
    }
    obj->sum = ((int64_t)value) * obj->value_count;
    obj->buffer_is_full = 1;
}

void moving_average_push_value(MovingAverage* obj, int32_t value) {
    obj->sum -= obj->values[obj->next_sample];
    obj->sum += value;
    obj->values[obj->next_sample] = value;
    obj->next_sample++;
    if (obj->next_sample >= obj->value_count) {
        obj->next_sample = 0;
        obj->buffer_is_full = 1;
    }
}
