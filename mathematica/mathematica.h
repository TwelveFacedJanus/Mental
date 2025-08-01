#ifndef mathematica_h
#define mathematica_h

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mental.h"

#define DEFINE_VEC(T) \
typedef struct Vec_##T { \
    T*       data; \
    size_t   size; \
    size_t   capacity; \
} Vec_##T

#define DECLARE_VEC_FUNCTIONS(T) \
MentalResult vec_init_##T(Vec_##T* vec, size_t initial_capacity); \
MentalResult vec_destroy_##T(Vec_##T* vec); \
MentalResult vec_push_back_##T(Vec_##T* vec, T value); \
MentalResult vec_pop_back_##T(Vec_##T* vec); \
MentalResult vec_reserve_##T(Vec_##T* vec, size_t new_capacity); \
MentalResult vec_resize_##T(Vec_##T* vec, size_t new_size, T default_value); \
T* vec_at_##T(const Vec_##T* vec, size_t index); \
const T* vec_const_at_##T(const Vec_##T* vec, size_t index); \
size_t vec_size_##T(const Vec_##T* vec); \
size_t vec_capacity_##T(const Vec_##T* vec); \
int vec_empty_##T(const Vec_##T* vec); \
void vec_clear_##T(Vec_##T* vec); \
MentalResult vec_insert_##T(Vec_##T* vec, size_t index, T value); \
MentalResult vec_erase_##T(Vec_##T* vec, size_t index);


#define DECLARE_MATH_STRUCTS(T) \
    DEFINE_VEC(T); \
    DECLARE_VEC_FUNCTIONS(T);


DECLARE_MATH_STRUCTS(float);
DECLARE_MATH_STRUCTS(int);
DECLARE_MATH_STRUCTS(double);
DECLARE_MATH_STRUCTS(char);


#endif // mathematica_h