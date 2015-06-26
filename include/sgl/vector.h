/*
 * Copyright (C) 2015 Morwenn
 *
 * The SGL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * The SGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program. If not,
 * see <http://www.gnu.org/licenses/>.
 */
#ifndef SGL_VECTOR_H_
#define SGL_VECTOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sgl/collection.h>
#include <sgl/detail/common.h>

/**
 * @def sgl_vector(T)
 *
 * Alias for the sgl_vector_T type.
 */
#define sgl_vector(T) \
    sgl_vector_##T

/**
 * @def sgl_define_sgl_vector(T)
 * Creates all the methods for a sgl_vector of given type.
 */
#define sgl_define_sgl_vector(T)                                                    \
                                                                                    \
    typedef struct sgl_detail_vector_##T sgl_vector(T);                             \
                                                                                    \
    typedef T sgl_vector_##T##_value_type;                                          \
    typedef size_t sgl_vector_##T##_size_type;                                      \
    typedef ptrdiff_t sgl_vector_##T##_difference_type;                             \
    typedef T* sgl_vector_##T##_iterator;                                           \
    typedef const T* sgl_vector_##T##_const_iterator;                               \
                                                                                    \
    typedef struct                                                                  \
    {                                                                               \
        void (*delete)(sgl_vector(T)*);                                             \
        T* (*at)(const sgl_vector(T)*, size_t);                                     \
        T* (*front)(const sgl_vector(T)*);                                          \
        T* (*back)(const sgl_vector(T)*);                                           \
        T* (*data)(const sgl_vector(T)*);                                           \
        T* (*begin)(sgl_vector(T)*);                                                \
        const T* (*cbegin)(const sgl_vector(T)*);                                   \
        T* (*end)(sgl_vector(T)*);                                                  \
        const T* (*cend)(const sgl_vector(T)*);                                     \
        bool (*is_empty)(const sgl_vector(T)*);                                     \
        size_t (*size)(const sgl_vector(T)*);                                       \
        size_t (*max_size)(void);                                                   \
        void (*reserve)(sgl_vector(T)*, size_t);                                    \
        size_t (*capacity)(const sgl_vector(T)*);                                   \
        void (*shrink_to_fit)(sgl_vector(T*));                                      \
        void (*clear)(sgl_vector(T)*);                                              \
        void (*erase1)(sgl_vector(T)*, const T*);                                   \
        void (*erase2)(sgl_vector(T)*, const T*, const T*);                         \
        void (*push_back)(sgl_vector(T)*, T);                                       \
        void (*pop_back)(sgl_vector(T)*);                                           \
        void (*resize1)(sgl_vector(T)*, size_t);                                    \
        void (*resize2)(sgl_vector(T)*, size_t, T);                                 \
        bool (*check)(const sgl_vector(T)*,                                         \
                      const char*,                                                  \
                      const sgl_vector(T)*);                                        \
    } sgl_detail_vector_functions_##T;                                              \
                                                                                    \
    struct sgl_detail_vector_##T                                                    \
    {                                                                               \
        T* _data;                                                                   \
        size_t _size;                                                               \
        size_t _capacity;                                                           \
        const sgl_detail_vector_functions_##T* _functions;                          \
    };                                                                              \
                                                                                    \
sgl_vector(T)* sgl_new_sgl_vector_##T();                                            \
void sgl_vector_delete_##T(sgl_vector(T)*);                                         \
T* sgl_vector_at_##T(const sgl_vector(T)*, size_t);                                 \
T* sgl_vector_front_##T(const sgl_vector(T)*);                                      \
T* sgl_vector_back_##T(const sgl_vector(T)*);                                       \
T* sgl_vector_data_##T(const sgl_vector(T)*);                                       \
T* sgl_vector_begin_##T(sgl_vector(T)*);                                            \
const T* sgl_vector_cbegin_##T(const sgl_vector(T)*);                               \
T* sgl_vector_end_##T(sgl_vector(T)*);                                              \
const T* sgl_vector_cend_##T(const sgl_vector(T)*);                                 \
bool sgl_vector_is_empty_##T(const sgl_vector(T)*);                                 \
size_t sgl_vector_size_##T(const sgl_vector(T)*);                                   \
size_t sgl_vector_max_size_##T(void);                                               \
void sgl_vector_reserve_##T(sgl_vector(T)*, size_t);                                \
size_t sgl_vector_capacity_##T(const sgl_vector(T)*);                               \
void sgl_vector_shrink_to_fit_##T(sgl_vector(T)*);                                  \
void sgl_vector_clear_##T(sgl_vector(T)*);                                          \
void sgl_vector_erase1_##T(sgl_vector(T)*, const T*);                               \
void sgl_vector_erase2_##T(sgl_vector(T)*, const T*, const T*);                     \
void sgl_vector_push_back_##T(sgl_vector(T)*, T);                                   \
void sgl_vector_pop_back_##T(sgl_vector(T)*);                                       \
void sgl_vector_resize1_##T(sgl_vector(T)*, size_t);                                \
void sgl_vector_resize2_##T(sgl_vector(T)*, size_t, T);                             \
bool sgl_vector_check_##T(const sgl_vector(T)*, const char*, const sgl_vector(T)*); \
                                                                                    \
void sgl_vector_delete_##T(sgl_vector(T)* vector)                                   \
{                                                                                   \
    free(vector->_data);                                                            \
    free(vector);                                                                   \
}                                                                                   \
                                                                                    \
T* sgl_vector_at_##T(const sgl_vector(T)* vector, size_t index)                     \
{                                                                                   \
    return sgl_data(vector) + index;                                                \
}                                                                                   \
                                                                                    \
T* sgl_vector_front_##T(const sgl_vector(T)* vector)                                \
{                                                                                   \
    return sgl_data(vector);                                                        \
}                                                                                   \
                                                                                    \
T* sgl_vector_back_##T(const sgl_vector(T)* vector)                                 \
{                                                                                   \
    return sgl_data(vector) + sgl_size(vector) - 1;                                 \
}                                                                                   \
                                                                                    \
T* sgl_vector_data_##T(const sgl_vector(T)* vector)                                 \
{                                                                                   \
    return vector->_data;                                                           \
}                                                                                   \
                                                                                    \
T* sgl_vector_begin_##T(sgl_vector(T)* vector)                                      \
{                                                                                   \
    return sgl_data(vector);                                                        \
}                                                                                   \
                                                                                    \
const T* sgl_vector_cbegin_##T(const sgl_vector(T)* vector)                         \
{                                                                                   \
    return sgl_data(vector);                                                        \
}                                                                                   \
                                                                                    \
T* sgl_vector_end_##T(sgl_vector(T)* vector)                                        \
{                                                                                   \
    return sgl_data(vector) + sgl_size(vector);                                     \
}                                                                                   \
                                                                                    \
const T* sgl_vector_cend_##T(const sgl_vector(T)* vector)                           \
{                                                                                   \
    return sgl_data(vector) + sgl_size(vector);                                     \
}                                                                                   \
                                                                                    \
bool sgl_vector_is_empty_##T(const sgl_vector(T)* vector)                           \
{                                                                                   \
    return sgl_size(vector) == 0;                                                   \
}                                                                                   \
                                                                                    \
size_t sgl_vector_size_##T(const sgl_vector(T)* vector)                             \
{                                                                                   \
    return vector->_size;                                                           \
}                                                                                   \
                                                                                    \
size_t sgl_vector_max_size_##T(void)                                                \
{                                                                                   \
    return SIZE_MAX;                                                                \
}                                                                                   \
                                                                                    \
void sgl_vector_reserve_##T(sgl_vector(T)* vector, size_t new_cap)                  \
{                                                                                   \
    if (new_cap > sgl_capacity(vector))                                             \
    {                                                                               \
        vector->_capacity = new_cap;                                                \
        vector->_data = realloc(vector->_data,                                      \
                                     new_cap * sizeof(T));                          \
    }                                                                               \
}                                                                                   \
                                                                                    \
size_t sgl_vector_capacity_##T(const sgl_vector(T)* vector)                         \
{                                                                                   \
    return vector->_capacity;                                                       \
}                                                                                   \
                                                                                    \
void sgl_vector_shrink_to_fit_##T(sgl_vector(T)* vector)                            \
{                                                                                   \
    vector->_capacity = sgl_size(vector);                                           \
    vector->_data = realloc(vector->_data,                                          \
                                 sgl_size(vector) * sizeof(T));                     \
}                                                                                   \
                                                                                    \
void sgl_vector_clear_##T(sgl_vector(T)* vector)                                    \
{                                                                                   \
    vector->_size = 0;                                                              \
}                                                                                   \
                                                                                    \
void sgl_vector_erase1_##T(sgl_vector(T)* vector, const T* pos)                     \
{                                                                                   \
    T* it = (T*) pos;                                                               \
    while (it != sgl_end(vector)-1)                                                 \
    {                                                                               \
        *it = it[1];                                                                \
        ++it;                                                                       \
    }                                                                               \
    --vector->_size;                                                                \
}                                                                                   \
                                                                                    \
void sgl_vector_erase2_##T(sgl_vector(T)* vector, const T* first, const T* last)    \
{                                                                                   \
    T* it1 = (T*) first;                                                            \
    T* it2 = (T*) last;                                                             \
    while (it2 != sgl_end(vector))                                                  \
    {                                                                               \
        *it1 = *it2;                                                                \
        ++it1;                                                                      \
        ++it2;                                                                      \
    }                                                                               \
    vector->_size -= last-first;                                                    \
}                                                                                   \
                                                                                    \
void sgl_vector_push_back_##T(sgl_vector(T)* vector, T value)                       \
{                                                                                   \
    vector->_size += 1;                                                             \
    if (sgl_capacity(vector) < sgl_size(vector))                                    \
    {                                                                               \
        sgl_reserve(vector, sgl_capacity(vector)+40);                               \
    }                                                                               \
    sgl_back(vector) = value;                                                       \
}                                                                                   \
                                                                                    \
void sgl_vector_pop_back_##T(sgl_vector(T)* vector)                                 \
{                                                                                   \
    if (sgl_size(vector) > 0)                                                       \
    {                                                                               \
        vector->_size -= 1;                                                         \
    }                                                                               \
}                                                                                   \
                                                                                    \
void sgl_vector_resize1_##T(sgl_vector(T)* vector, size_t count)                    \
{                                                                                   \
    if (count < sgl_size(vector))                                                   \
    {                                                                               \
        while (sgl_size(vector) > count)                                            \
        {                                                                           \
            sgl_pop_back(vector);                                                   \
        }                                                                           \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        sgl_reserve(vector, count);                                                 \
        vector->_size = count;                                                      \
    }                                                                               \
}                                                                                   \
                                                                                    \
void sgl_vector_resize2_##T(sgl_vector(T)* vector, size_t count, T value)           \
{                                                                                   \
    if (count < sgl_size(vector))                                                   \
    {                                                                               \
        while (sgl_size(vector) > count)                                            \
        {                                                                           \
            sgl_pop_back(vector);                                                   \
        }                                                                           \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        while (sgl_size(vector) < count)                                            \
        {                                                                           \
            sgl_push_back(vector, value);                                           \
        }                                                                           \
    }                                                                               \
}                                                                                   \
                                                                                    \
bool sgl_vector_check_##T(const sgl_vector(T)* lhs,                                 \
                          const char* op,                                           \
                          const sgl_vector(T)* rhs)                                 \
{                                                                                   \
    if (not strcmp(op, "=="))                                                       \
    {                                                                               \
        for (size_t i = 0 ; i < sgl_size(lhs) ; ++i)                                \
        {                                                                           \
            if (sgl_data(lhs)[i] != sgl_data(rhs)[i])                               \
            {                                                                       \
                return false;                                                       \
            }                                                                       \
        }                                                                           \
        return true;                                                                \
    }                                                                               \
    else if (not strcmp(op, "!="))                                                  \
    {                                                                               \
        for (size_t i = 0 ; i < sgl_size(lhs) ; ++i)                                \
        {                                                                           \
            if (sgl_data(lhs)[i] != sgl_data(rhs)[i])                               \
            {                                                                       \
                return true;                                                        \
            }                                                                       \
        }                                                                           \
        return false;                                                               \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        /* unknown op */                                                            \
    }                                                                               \
    return false; /* should never be used */                                        \
}                                                                                   \
                                                                                    \
const sgl_detail_vector_functions_##T sgl_detail_vector_funcs_##T = {               \
    &sgl_vector_delete_##T,                                                         \
    &sgl_vector_at_##T,                                                             \
    &sgl_vector_front_##T,                                                          \
    &sgl_vector_back_##T,                                                           \
    &sgl_vector_data_##T,                                                           \
    &sgl_vector_begin_##T,                                                          \
    &sgl_vector_cbegin_##T,                                                         \
    &sgl_vector_end_##T,                                                            \
    &sgl_vector_cend_##T,                                                           \
    &sgl_vector_is_empty_##T,                                                       \
    &sgl_vector_size_##T,                                                           \
    &sgl_vector_max_size_##T,                                                       \
    &sgl_vector_reserve_##T,                                                        \
    &sgl_vector_capacity_##T,                                                       \
    &sgl_vector_shrink_to_fit_##T,                                                  \
    &sgl_vector_clear_##T,                                                          \
    &sgl_vector_erase1_##T,                                                         \
    &sgl_vector_erase2_##T,                                                         \
    &sgl_vector_push_back_##T,                                                      \
    &sgl_vector_pop_back_##T,                                                       \
    &sgl_vector_resize1_##T,                                                        \
    &sgl_vector_resize2_##T,                                                        \
    &sgl_vector_check_##T                                                           \
};                                                                                  \
                                                                                    \
sgl_vector(T)* sgl_new_sgl_vector_##T()                                             \
{                                                                                   \
    sgl_vector_##T* res = malloc(sizeof(sgl_vector(T)));                            \
    res->_functions = &sgl_detail_vector_funcs_##T;                                 \
    res->_capacity = 40;                                                            \
    res->_size = 0;                                                                 \
    res->_data = malloc(res->_capacity * sizeof(T));                                \
    return res;                                                                     \
}

#endif // SGL_VECTOR_H_
