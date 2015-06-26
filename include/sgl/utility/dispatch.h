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
#ifndef SGL_UTILITY_DISPATCH_H_
#define SGL_UTILITY_DISPATCH_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <sgl/detail/common.h>
#include <sgl/utility/paste.h>
#include <sgl/utility/va_nargs.h>

// Note: sgl_dispatch cannot use sgl_va_nargs because of the additional
//       sizeof check. Therefore, it uses the version which cannot handle
//       0-parameter overloads.

/**
 * @def sgl_dispatch(func, ...)
 *
 * Calls different functions depending on the number of parameters
 * in the parameter pack. It allows to "overload" macros based on the
 * number of arguments passed to it.
 *
 * @warning It cannot handle 0-sized parameter packs.
 */
#define sgl_dispatch(func, ...) \
    sgl_paste(func, sgl_va_nargs1(__VA_ARGS__))

#endif // SGL_UTILITY_DISPATCH_H_
