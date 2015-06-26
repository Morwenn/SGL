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
#ifndef SGL_UTILITY_VA_NARGS_H_
#define SGL_UTILITY_VA_NARGS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <sgl/detail/common.h>

#define sgl_va_nargs2(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, N, ...) \
    N

// Note: the last dummy parameter sole purpose is to avoid
//       a warning from the compiler
#define sgl_va_nargs1(...) \
    sgl_va_nargs2(__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, /* dummy */)

/**
 * @def sgl_va_nargs(...)
 *
 * Counts the number of arguments in the given macro parameter pack.
 *
 * @warning The result cannot be used for token pasting.
 */
#define sgl_va_nargs(...) \
    (sizeof(#__VA_ARGS__) == sizeof("") ? 0 : \
    sgl_va_nargs1(__VA_ARGS__))

#endif // SGL_UTILITY_VA_NARGS_H_
