/*
 * Copyright (C) 2019 Morwenn
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
#ifndef SGL_ALGORITHM_H_
#define SGL_ALGORITHM_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/// A decent part of the STL algorithm library. Note that to keep code
/// small, most of these are found in src/algorithm.c. This may be
/// changed in the future to inline functions.
/// Additionally, these are implemented like qsort and bsearch.
/// For example:
///    bool less(const Type &t);
///    std::next_permutation(data.begin(), data.end(), less);
/// would become
///    int compare(const void *a, const void *b);
///    sgl_next_permutation(data, count, sizeof(data[0]), compare);


/// A struct that holds two pointers.
/// These may point to const data.
typedef struct {
    void *first;
    void *second;
} sgl_iter_pair_t;

/// A typedef to represent a pointer to a function that satisfies the following:
///   - Compares in1 and in2, pointers to elements, against each other.
///     - If in1 is less than in2, return a negative number.
///     - If in1 is equal to in2, return 0.
///     - If in1 is greater than in2, return a negative number.
///   - in1 and in2 must not be modified.
///   - Only up to size given to the algorithm bytes will be read.
///   - This is the same kind of function that is used for C style comparators
///     such as ones to be given to qsort.
typedef int (*sgl_c_comparator_t)(const void *in1, const void *in2);

/// A typedef to represent a pointer to a function that satisfies the following:
///   - Checks in, a pointer to an elememt, against some condition.
///     - If the condition is met, return true.
///     - Otherwise, return false.
///   - in must not be modified.
///   - Only up to size given to the algorithm bytes will be read.
typedef bool (*sgl_unary_pred_t)(const void *in);

/// A typedef to represent a pointer to a function that satisfies the following:
///   - Checks in1 and in2, pointers to elements, against some condition.
///     - If the condition is met, return true.
///     - Otherwise, return false.
///   - in1 and in2 must not be modified.
///   - Only up to size given to the algorithm bytes will be read.
typedef bool (*sgl_binary_pred_t)(const void *in1, const void *in2);

/// A typedef to represent a pointer to a function that satisfies the following:
///   - Writes or modifies an element pointed to by out.
///   - Only up to size given to the algorithm bytes will be read or written.
typedef void (*sgl_transformer_t)(void *out);

/// A typedef to represent a pointer to a function that satisfies the following:
///   - Writes or modifies an element pointed to by out.
///   - Accepts in, a pointer to an element, as input.
///   - in must not point to out.
///   - in must not be modified.
///   - Only up to size given to the algorithm bytes will be read or written.
typedef void (*sgl_unary_transformer_t)(void *restrict out, const void *in);

/// A typedef to represent a pointer to a function that satisfies the following:
///   - Writes or modifies an element pointed to by out.
///   - Accepts in1 and in2, pointers to elememts, as input.
///   - in1 and in2 must not point to out.
///   - in1 and in2 must not be modified.
///   - Only up to size given to the algorithm bytes will be read or written.
typedef void (*sgl_binary_transformer_t)(void *restrict out, const void *in1, const void *in2);


/// Returns the max element of a series, or NULL if size or count is zero.
///   - series:  The data to search. It must be at least count * size bytes large.
///   - count:   The number of elements in series.
///   - size:    The size of each element in series. It is an error for this
///              to be zero.
///   - compare: The C-style comparator to check values against.
///   - Returns: A pointer to the max element in series, with const casted away.
///              This may still point to const data.
///
/// Note: series, count, and size will be left out from further comments because
/// their usages don't usually change.
void *sgl_max_element(const void *series, size_t count, size_t size, sgl_c_comparator_t compare);

/// Returns the min element of a series, or NULL if size is zero.
/// See sgl_max_element.
void *sgl_min_element(const void *series, size_t count, size_t size, sgl_c_comparator_t compare);

/// Swaps size bytes from a and b.
///   - a should not equal b.
void sgl_iter_swap(void *restrict a, void *restrict b, size_t size);

/// Reverses elements of size bytes between begin and end. One of the few STL
/// style functions here.
///    - begin: The start of the range
void sgl_reverse_range(void *begin, void *end, size_t size);

/// Reverses series.
static inline void sgl_reverse(void *series, size_t count, size_t size)
{
    uint8_t *first = series;
    uint8_t *last = first + (count * size);
    sgl_reverse_range(first, last, size);
}

/// Returns two pointers pointing to where series1 and series2 don't match.
/// or { NULL, NULL } if it could not be found. The const is casted away.
sgl_iter_pair_t sgl_mismatch(const void *series1, const void *series2,
                             size_t count, size_t size,
                             sgl_c_comparator_t compare);

/// Returns the index of where series1 and series2 don't match.
size_t sgl_mismatch_count(const void *series1, const void *series2,
                          size_t count, size_t size,
                          sgl_c_comparator_t compare);


/// Finds the first element pointed to by val in series, or NULL if it can't
/// be found.
///   - const is casted away.
///   - The values are compared with memcmp.
void *sgl_find_raw(const void *series, size_t count, size_t size, const void *val);

/// Finds the first element pointed to by val in series, or NULL if it can't
/// be found.
///   - const is casted away.
void *sgl_find(const void *series, size_t count, size_t size, const void *val, sgl_c_comparator_t compare);

/// Finds the first element that satisfies pred in series, or NULL if it can't
/// be found.
///   - const is casted away.
///   - The values are checked with the unary predicate pred.
void *sgl_find_if(const void *series, size_t count, size_t size,
                  sgl_unary_pred_t pred);

/// Finds the first element that does NOT satisfy pred in series, or NULL if it
/// can't be found.
///   - const is casted away.
///   - The values are checked with the unary predicate pred.
void *sgl_find_if_not(const void *series, size_t count, size_t size,
                      sgl_unary_pred_t pred);

/// Finds the first element in series that matches any of the vals_count elements
/// in vals.
///   - const is casted away.
///   - The elements are compared with memcmp.
void *sgl_find_first_of(const void *series, size_t count, size_t size, const void *vals, size_t vals_count);

/// Finds the first element in series which any of the vals_count elements in vals
/// satisfies the binary predicate pred.
///   - const is casted away.
void *sgl_find_first_of_if(const void *series, size_t count, size_t size, const void *vals, size_t vals_count, sgl_binary_pred_t pred);

/// Counts the number of elements in series that match val.
///   - The elements are compared with memcmp.
size_t sgl_count(const void *series, size_t count, size_t size, const void *val, sgl_c_comparator_t compare);

/// Counts the number of elements in series that satisfy the unary predicate pred.
size_t sgl_count_if(const void *series, size_t count, size_t size, sgl_unary_pred_t pred);

/// Returns a pointer to the first instance where two elements match or NULL
void *sgl_adjacent_find(const void *series, size_t count, size_t size, sgl_c_comparator_t compare);

/// Returns a pointer to the first instance where two elements satisfy the
/// binary predicate pred, or NULL.
void *sgl_adjacent_find_if(const void *series, size_t count, size_t size, sgl_binary_pred_t pred);

/// Returns true if all of the elements in series match the unary predicate pred.
static inline bool sgl_all_of(const void *series, size_t count, size_t size, sgl_unary_pred_t pred)
{
    return sgl_find_if_not(series, count, size, pred) == NULL;
}

/// Returns true if any of the elements in series match the unary predicate pred.
static inline bool sgl_any_of(const void *series, size_t count, size_t size, sgl_unary_pred_t pred)
{
    return sgl_find_if(series, count, size, pred) != NULL;
}

/// Returns true if none of the elements in series match the unary predicate pred.
static inline bool sgl_none_of(const void *series, size_t count, size_t size, sgl_unary_pred_t pred)
{
    return sgl_find_if(series, count, size, pred) == NULL;
}

/// Removes all of the elements in series that match val.
/// val must not point to an element in series.
void *sgl_remove(void *restrict series, size_t count, size_t size, const void *restrict val, sgl_c_comparator_t compare);

/// Removes all of the elements in series that satisfy the unary predicate pred.
void *sgl_remove_if(void *restrict series, size_t count, size_t size, sgl_unary_pred_t pred);

/// Calls func on each element in series. This assumes a mutable series.
void sgl_for_each_mut(void *series, size_t count, size_t size, void (*func)(void *));


/// Calls func on each element in series. This assumes a const series.
void sgl_for_each_const(const void *series, size_t count, size_t size, void (*func)(const void *));

#define sgl_for_each(series, count, size, func) \
    _Generic((series), \
        void *: _Generic((func), \
             void (*)(const void *): sgl_for_each_const, \
             void (*)(void *):       sgl_for_each_mut \
        ), \
        const void *: sgl_for_each_const \
    )((series),(count),(size),(func))

/// Replaces all elements in series which match old_value with new_value.
void sgl_replace(void *restrict series, size_t count, size_t size, const void *restrict old_value, const void *restrict new_value, sgl_c_comparator_t compare);
/// Replaces all elements in series which satisfy pred with new_value.
void sgl_replace_if(void *restrict series, size_t count, size_t size, const void *restrict new_value, sgl_unary_pred_t pred);

/// Applies the unary transformer tran to each element of out using the
/// corresponding element of in as input.
///   - in and out must not overlap.
void sgl_transform_unary(
    void *restrict out,
    const void *restrict in,
    size_t count,
    size_t size,
    sgl_unary_transformer_t tran
);


/// Applies the binary transformer tran to each element of out using the
/// corresponding elements of in1 and in2 as input.
///   - in1 and in2 must not overlap with out.
void sgl_transform_binary(
    void *restrict out,
    const void *in1,
    const void *in2,
    size_t count,
    size_t size,
    sgl_binary_transformer_t tran
);

/// Fills each element of out with the element pointed to by in.
/// This has a similar effect to memset, and when size is 1, memset is called.
/// Otherwise, each element is copied with memcpy.
///   - in must not point to out.
void sgl_fill(void *restrict out, size_t count, size_t size, const void *restrict in);

/// Applies the value generated by generator to each element in series.
///   - These are assumed to be uninitialized.
void sgl_generate(void *series, size_t count, size_t size, sgl_transformer_t generator);

/// Partitons series based on whether it matches the unary predicate pred
void *sgl_partition_if(void *series, size_t count, size_t size, sgl_unary_pred_t pred);

/// Partitons series based on whether it is less than pivot.
void *sgl_partition(void *restrict series, size_t count, size_t size, const void *restrict pivot, sgl_c_comparator_t compare);

/// Permutes the next permutation in a series, returning true if it can be created.

bool sgl_next_permutation(void *series, size_t count, size_t size, sgl_c_comparator_t compare);

/// Permutes the previous permutation in a series, returning true if it can be created.
bool sgl_prev_permutation(void *series, size_t count, size_t size, sgl_c_comparator_t compare);

#endif // SGL_ALGORITHM_H_
