#include "mathematica.h"

#define IMPLEMENT_VEC_FUNCTIONS(T) \
MentalResult vec_init_##T(Vec_##T* vec, size_t initial_capacity) { \
    if (!vec) return MENTAL_VECTOR_POINTER_IS_NULL; \
    vec->data = (initial_capacity > 0) ? (T*)malloc(sizeof(T) * initial_capacity) : NULL; \
    if (initial_capacity > 0 && !vec->data) return MENTAL_VECTOR_OUT_OF_MEMORY; \
    vec->size = 0; \
    vec->capacity = initial_capacity; \
    return MENTAL_OK; \
} \
\
MentalResult vec_destroy_##T(Vec_##T* vec) { \
    if (!vec) return MENTAL_VECTOR_POINTER_IS_NULL; \
    free(vec->data); \
    vec->data = NULL; \
    vec->size = 0; \
    vec->capacity = 0; \
    return MENTAL_OK; \
} \
\
MentalResult vec_push_back_##T(Vec_##T* vec, T value) { \
    if (!vec) return MENTAL_VECTOR_POINTER_IS_NULL; \
    if (vec->size >= vec->capacity) { \
        size_t new_capacity = vec->capacity == 0 ? 1 : vec->capacity * 2; \
        T* new_data = (T*)realloc(vec->data, sizeof(T) * new_capacity); \
        if (!new_data) return MENTAL_VECTOR_OUT_OF_MEMORY; \
        vec->data = new_data; \
        vec->capacity = new_capacity; \
    } \
    vec->data[vec->size++] = value; \
    return MENTAL_OK; \
} \
\
MentalResult vec_pop_back_##T(Vec_##T* vec) { \
    if (!vec) return MENTAL_VECTOR_POINTER_IS_NULL; \
    if (vec->size == 0) return MENTAL_VECTOR_UNDERFLOW; \
    vec->size--; \
    return MENTAL_OK; \
} \
\
MentalResult vec_reserve_##T(Vec_##T* vec, size_t new_capacity) { \
    if (!vec) return MENTAL_VECTOR_POINTER_IS_NULL; \
    if (new_capacity > vec->capacity) { \
        T* new_data = (T*)realloc(vec->data, sizeof(T) * new_capacity); \
        if (!new_data) return MENTAL_VECTOR_OUT_OF_MEMORY; \
        vec->data = new_data; \
        vec->capacity = new_capacity; \
    } \
    return MENTAL_OK; \
} \
\
MentalResult vec_resize_##T(Vec_##T* vec, size_t new_size, T default_value) { \
    if (!vec) return MENTAL_VECTOR_POINTER_IS_NULL; \
    if (new_size > vec->capacity) { \
        MentalResult res = vec_reserve_##T(vec, new_size); \
        if (res != MENTAL_OK) return res; \
    } \
    if (new_size > vec->size) { \
        for (size_t i = vec->size; i < new_size; i++) { \
            vec->data[i] = default_value; \
        } \
    } \
    vec->size = new_size; \
    return MENTAL_OK; \
} \
\
T* vec_at_##T(const Vec_##T* vec, size_t index) { \
    if (!vec || index >= vec->size) return NULL; \
    return &vec->data[index]; \
} \
\
const T* vec_const_at_##T(const Vec_##T* vec, size_t index) { \
    if (!vec || index >= vec->size) return NULL; \
    return &vec->data[index]; \
} \
\
size_t vec_size_##T(const Vec_##T* vec) { \
    return vec ? vec->size : 0; \
} \
\
size_t vec_capacity_##T(const Vec_##T* vec) { \
    return vec ? vec->capacity : 0; \
} \
\
int vec_empty_##T(const Vec_##T* vec) { \
    return !vec || vec->size == 0; \
} \
\
void vec_clear_##T(Vec_##T* vec) { \
    if (vec) vec->size = 0; \
} \
\
MentalResult vec_insert_##T(Vec_##T* vec, size_t index, T value) { \
    if (!vec || index > vec->size) return MENTAL_VECTOR_POINTER_IS_NULL; \
    if (vec->size >= vec->capacity) { \
        size_t new_capacity = vec->capacity == 0 ? 1 : vec->capacity * 2; \
        T* new_data = (T*)realloc(vec->data, sizeof(T) * new_capacity); \
        if (!new_data) return MENTAL_VECTOR_OUT_OF_MEMORY; \
        vec->data = new_data; \
        vec->capacity = new_capacity; \
    } \
    memmove(&vec->data[index+1], &vec->data[index], sizeof(T) * (vec->size - index)); \
    vec->data[index] = value; \
    vec->size++; \
    return MENTAL_OK; \
} \
\
MentalResult vec_erase_##T(Vec_##T* vec, size_t index) { \
    if (!vec || index >= vec->size) return MENTAL_VECTOR_POINTER_IS_NULL; \
    if (index < vec->size - 1) { \
        memmove(&vec->data[index], &vec->data[index+1], sizeof(T) * (vec->size - index - 1)); \
    } \
    vec->size--; \
    return MENTAL_OK; \
}

IMPLEMENT_VEC_FUNCTIONS(float)
IMPLEMENT_VEC_FUNCTIONS(int)
IMPLEMENT_VEC_FUNCTIONS(double)