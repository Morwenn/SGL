/*
 * Copyright (C) 2019 easyaspi314
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
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <sgl/algorithm.h>

size_t sgl_count(const void *series, size_t count, size_t size, const void *val, sgl_c_comparator_t compare)
{
    if (count == 0 || size == 0)
        return 0;
    size_t matches = 0;
    const uint8_t *iter = series;
    while (count-- > 0) {
        if (compare(iter, val) == 0)
            ++matches;
        iter += size;
    }
    return matches;
}

size_t sgl_count_if(const void *series, size_t count, size_t size, sgl_unary_pred_t pred)
{
    if (count == 0 || size == 0)
        return 0;
    size_t matches = 0;
    const uint8_t *iter = series;
    while (count-- > 0) {
        if (pred(iter))
            ++matches;
        iter += size;
    }
    return matches;
}

void *sgl_max_element(const void *series, size_t count, size_t size, sgl_c_comparator_t compare)
{
    if (count == 0 || size == 0)
        return NULL;
    const uint8_t *largest = series;
    const uint8_t *i = largest + size;
    while (count-- > 0) {
        if (compare(largest, i) < 0)
            largest = i;
        i += size;
    }
    // const cast intentional
    return (void *)largest;
}

void *sgl_min_element(const void *series, size_t count, size_t size, sgl_c_comparator_t compare)
{
    if (count == 0 || size == 0)
        return NULL;
    const uint8_t *smallest = series;
    const uint8_t *i = smallest + size;
    while (count-- > 0) {
        if (compare(smallest, i) > 0)
            smallest = i;
        i += size;
    }
    // const cast intentional
    return (void *)smallest;
}

void sgl_iter_swap(void *restrict a, void *restrict b, size_t size)
{
    if (a != b && size != 0) {
        uint8_t *lhs = a;
        uint8_t *rhs = b;
        size_t i = 0;
        size_t end = size & ~(sizeof(uintptr_t) - 1);
        // Increase our bandwidth with larger aligned input
        if (end && ((uintptr_t)a & (sizeof(uintptr_t) - 1)) == 0 && ((uintptr_t)b & (sizeof(uintptr_t) - 1)) == 0) {
            do {
                uintptr_t tmp = *(uintptr_t *)(rhs + i);
                *(uintptr_t *)(rhs + i) = *(uintptr_t *)(lhs + i);
                *(uintptr_t *)(lhs + i) = tmp;
                i += sizeof(uintptr_t);
            } while (i < end);
        }
        for (; i < size; i++) {
           uint8_t tmp = rhs[i];
           rhs[i] = lhs[i];
           lhs[i] = tmp;
        }
    }
}

void sgl_reverse_range(void *begin, void *end, size_t size)
{
    if (size == 0)
        return;
    uint8_t *first = begin;
    uint8_t *last = end;

    while ((first < last) && (first < (last -= size))) {
        sgl_iter_swap(first, last, size);
        first += size;
    }
}

sgl_iter_pair_t sgl_mismatch(
    const void *series1,
    const void *series2,
    size_t count,
    size_t size,
    int (*compare)(const void *, const void *)
)
{
     if (size == 0 || series1 == series2) // don't compare if equal
          return (sgl_iter_pair_t) { NULL, NULL };
     const uint8_t *first1 = series1;
     const uint8_t *first2 = series2;
     while (count-- > 0) {
         if (compare(first1, first2) != 0) {
             return (sgl_iter_pair_t) { (void *)first1, (void *)first2 };
         }
         first1 += size;
         first2 += size;
    }
    return (sgl_iter_pair_t) { NULL, NULL };
}

size_t sgl_mismatch_count(
    const void *series1,
    const void *series2,
    size_t count,
    size_t size,
    int (*compare)(const void *, const void *)
)
{
     if (size == 0 || series1 == series2) // don't compare if equal
          return (size_t)-1;
     const uint8_t *first1 = series1;
     const uint8_t *first2 = series2;
     for (size_t i = 0; i < count; i++) {
         if (compare(first1, first2) != 0) {
             return i;
         }
         first1 += size;
         first2 += size;
    }
    return (size_t)-1;
}


void sgl_copy_backward(void *restrict out, const void *restrict in, size_t count, size_t size)
{
    if (count == 0 || size == 0)
        return;
    uint8_t *out_iter = out;
    const uint8_t *in_iter = in;
    out_iter += count * size;
    in_iter += count * size;
    while (count-- > 0) {
         memcpy((out_iter -= size), (in_iter -= size), size);
    }
}

void sgl_move_backward(void *restrict out, void *restrict in, size_t count, size_t size)
{
    if (count == 0 || size == 0)
        return;
    uint8_t *out_iter = out;
    uint8_t *in_iter = in;
    out_iter += count * size;
    in_iter += count * size;
    while (count-- > 0) {
         memmove((out_iter -= size), (in_iter -= size), size);
    }
}


void *sgl_find_raw(const void *series, size_t count, size_t size, const void *val)
{
    if (count == 0 || size == 0) return NULL;
    if (size == 1) {
        const uint8_t *value = val;
        return memchr(series, value[0], count);
    }

    const uint8_t *iter = series;
    while (count-- > 0) {
        if (memcmp(iter, val, size) == 0)
            // const cast intentional
            return (void *)iter;
        iter += size;
    }
    return NULL;
}

void *sgl_find(const void *series, size_t count, size_t size, const void *val, sgl_c_comparator_t compare)
{
    if (count == 0 || size == 0) return NULL;

    const uint8_t *iter = series;
    while (count-- > 0) {
        if (compare(iter, val) == 0)
            // const cast intentional
            return (void *)iter;
        iter += size;
    }
    return NULL;
}

void *sgl_find_if(const void *series, size_t count, size_t size, sgl_unary_pred_t pred)
{
    if (count == 0 || size == 0)
        return NULL;
    const uint8_t *iter = series;
    while (count-- > 0) {
        if (pred(iter))
            // const cast intentional
            return (void *)iter;
        iter += size;
    }
    return NULL;
}

void *sgl_find_if_not(const void *series, size_t count, size_t size, sgl_unary_pred_t pred)
{
    if (count == 0 || size == 0)
        return NULL;
    const uint8_t *iter = series;
    while (count-- > 0) {
        if (!pred(iter))
            // const cast intentional
            return (void *)iter;
        iter += size;
    }
    return NULL;
}


void *sgl_find_first_of(const void *series, size_t count, size_t size, const void *vals, size_t vals_count)
{
    if (count == 0 || size == 0 || vals_count == 0)
        return NULL;
    const uint8_t *iter = series;
    while (count-- > 0) {
        const uint8_t *val_iter = vals;
        for (size_t i = 0; i < vals_count; i++) {
            if (memcmp(iter, val_iter, size) == 0)
                // const cast intentional
                return (void *)iter;
            val_iter += size;
        }
        iter += size;
    }
    return NULL;
}

void *sgl_find_first_of_if(const void *series, size_t count, size_t size, const void *vals, size_t vals_count, sgl_binary_pred_t pred)
{
    if (count == 0 || size == 0 || vals_count == 0)
        return NULL;
    const uint8_t *iter = series;
    while (count-- > 0) {
        const uint8_t *val_iter = vals;
        for (size_t i = 0; i < vals_count; i++) {
            if (pred(iter, val_iter))
                // const cast intentional
                return (void *)iter;
            val_iter += size;
        }
        iter += size;
    }
    return NULL;
}


void *sgl_adjacent_find(const void *series, size_t count, size_t size, sgl_c_comparator_t compare)
{
    if (count < 2 || size == 0) return NULL;
    const uint8_t *iter = series;
    const uint8_t *next = iter + size;
    --count;
    while (count-- > 0) {
        if (compare(iter, next) == 0)
            // const cast intentional
            return (void *)iter;
        iter += size;
        next += size;
    }
    return NULL;
}


void *sgl_adjacent_find_if(const void *series, size_t count, size_t size, sgl_binary_pred_t pred)
{
    if (count < 2 || size == 0) return NULL;
    const uint8_t *iter = series;
    const uint8_t *next = iter + size;
    --count;
    while (count-- > 0) {
        if (pred(iter, next))
            // const cast intentional
            return (void *)iter;
        iter += size;
        next += size;
    }
    return NULL;
}

// copied from cppreference
void *sgl_remove(void *restrict series, size_t count, size_t size, const void *restrict val, sgl_c_comparator_t compare)
{
    if (count == 0 || size == 0)
        return NULL;
    uint8_t *first = sgl_find(series, count, size, val, compare);
    if (first != NULL) {
        uint8_t *last = (uint8_t *)series + (count * size);
        for (uint8_t *i = first; (i += size) < last; ) {
            if (compare(i, val) != 0) {
                memmove(first, i, size);
                first += size;
            }
        }
        return first;
    }
    return first + (count * size);
}

// copied from cppreference
void *sgl_remove_if(void *restrict series, size_t count, size_t size, sgl_unary_pred_t pred)
{
    if (count == 0 || size == 0)
        return NULL;
    uint8_t *first = sgl_find_if(series, count, size, pred);
    if (first != NULL) {
        uint8_t *last = (uint8_t *)series + (count * size);
        for (uint8_t *i = first; (i += size) < last; ) {
            if (pred(i)) {
                memmove(first, i, size);
                first += size;
            }
        }
        return first;
    }
    return first + (count * size);
}

// copied from cppreference
void sgl_for_each_mut(void *series, size_t count, size_t size, void (*func)(void *))
{
    if (count == 0 || size == 0)
        return;
    uint8_t *first = series;
    while (count-- > 0) {
        func(first);
        first += size;
    }
}

// copied from cppreference
void sgl_for_each_const(const void *series, size_t count, size_t size, void (*func)(const void *))
{
    if (count == 0 || size == 0)
        return;
    const uint8_t *first = series;
    while (count-- > 0) {
        func(first);
        first += size;
    }
}

void sgl_replace(void *restrict series, size_t count, size_t size, const void *restrict old_value, const void *restrict new_value, sgl_c_comparator_t compare)
{
    if (count == 0 || size == 0)
        return;
    uint8_t *first = series;
    while (count-- > 0) {
        if (compare(first, old_value) == 0)
            memcpy(first, new_value, size);
        first += size;
    }
}

void sgl_replace_if(void *restrict series, size_t count, size_t size, const void *restrict new_value, sgl_unary_pred_t pred)
{
    if (count == 0 || size == 0)
        return;
    uint8_t *first = series;
    while (count-- > 0) {
        if (pred(first))
            memcpy(first, new_value, size);
        first += size;
    }
}

// copied from cppreference
void sgl_transform_unary(
    void *restrict out,
    const void *in,
    size_t count,
    size_t size,
    sgl_unary_transformer_t tran
)
{
    if (count == 0 || size == 0)
        return;
    uint8_t *first1 = out;
    const uint8_t *first2 = in;
    while (count-- > 0) {
        tran(first1, first2);
        first1 += size;
        first2 += size;
    }
}

// copied from cppreference
void sgl_transform_binary(
    void *restrict out,
    const void *in1,
    const void *in2,
    size_t count,
    size_t size,
    sgl_binary_transformer_t tran
)
{
    if (count == 0 || size == 0) return;
    uint8_t *first1 = out;
    const uint8_t *first2 = in1;
    const uint8_t *first3 = in2;
    while (count-- > 0) {
        tran(first1, first2, first3);
        first1 += size;
        first2 += size;
        first3 += size;
    }
}

void sgl_fill(void *restrict out, size_t count, size_t size, const void *restrict in)
{
    if (count == 0 || size == 0) return;
    uint8_t *first = out;
    const uint8_t *input = in;
    if (size == 1) { // memset is almost always faster here
        memset(out, input[0], count);
        return;
    }
    while (count-- > 0) {
        memcpy(first, in, size);
        first += size;
    }
}


// copied from cppreference
void sgl_generate(void *series, size_t count, size_t size, sgl_transformer_t generator)
{
    if (count == 0 || size == 0) return;
    uint8_t *first = series;
    while (count-- > 0) {
        generator(first);
        first += size;
    }
}

void *sgl_partition_if(void *series, size_t count, size_t size, sgl_unary_pred_t pred)
{
    if (count == 0 || size == 0)
        return NULL;

    uint8_t *first = sgl_find_if_not(series, count, size, pred);
    uint8_t *last = (uint8_t *)series + (count * size);

    if (first == NULL)
        return first;

    for (uint8_t *i = first + size; i < last; i += size) {
        if (pred(i)) {
           sgl_iter_swap(i, first, size);
           first += size;
        }
    }
    return first;
}


void *sgl_partition(void *restrict series, size_t count, size_t size, const void *restrict pivot, sgl_c_comparator_t compare)
{
    if (count == 0 || size == 0)
        return NULL;
    // TODO: early skip
    uint8_t *first = series; // sgl_find_if_not(series, count, size, pred);
    uint8_t *last = (uint8_t *)series + (count * size);

//    if (first == NULL)
//        return first;

    for (uint8_t *i = first + size; i < last; i += size) {
        if (compare(i, pivot) < 0) {
           sgl_iter_swap(i, first, size);
           first += size;
        }
    }
    return first;
}

bool sgl_is_permutation(const void *series1, const void *series2, size_t count, size_t size, sgl_c_comparator_t compare)
{
    if (count == 0 || size == 0) return false;
    // skip common prefix
    size_t skip = sgl_mismatch_count(series1, series2, count, size, compare);
    // iterate over the rest, counting how many times each element
    // from [first, last) appears in [d_first, d_last)
    if (skip != (size_t)-1) {
        count -= skip;
        const uint8_t *first = (const uint8_t *)series1 + (skip * size);
        const uint8_t *iter = first;
        const uint8_t *d_first = (const uint8_t *)series2 + (skip * size);
        for (size_t i = 0; i < count; ++i) {
            if (sgl_find(first, i, size, iter, compare) != iter) continue; // already counted this *i
            size_t m = sgl_count(d_first, count, size, iter, compare);
            if (m==0 || sgl_count(iter, count - i, size, iter, compare) != m) {
                return false;
            }
            iter += size;
        }
    }
    return true;
}

// copied from cppreference
bool sgl_next_permutation(void *series, size_t count, size_t size, sgl_c_comparator_t compare)
{
    if (count == 0 || size == 0)
        return false;
    uint8_t *first = series;
    uint8_t *last = first + (size * count);

    uint8_t *i = last;
    if (first >= (i -= size)) return false;
    for (;;) {
       uint8_t *i1, *i2;
       i1 = i;
       if (compare((i -= size), i1) < 0) {
           i2 = last;
           while (compare(i, (i2 -= size)) >= 0)
               ;
           sgl_iter_swap(i, i2, size);
           sgl_reverse_range(i1, last, size);
           return true;
       }
       if (i == first) {
           sgl_reverse_range(first, last, size);
           return false;
       }
   }
}

// copied from cppreference
bool sgl_prev_permutation(void *series, size_t count, size_t size, sgl_c_comparator_t compare)
{
    if (count == 0 || size == 0)
        return false;
    uint8_t *first = series;
    uint8_t *last = first + (size * count);

    uint8_t *i = last;
    if (first >= (i -= size)) return false;
    for (;;) {
       uint8_t *i1, *i2;
       i1 = i;
       if (compare(i1, (i -= size)) < 0) {
           i2 = last;
           while (compare((i2 -= size), i) >= 0)
               ;
           sgl_iter_swap(i, i2, size);
           sgl_reverse_range(i1, last, size);
           return true;
       }
       if (i == first) {
           sgl_reverse_range(first, last, size);
           return false;
       }
   }
}
