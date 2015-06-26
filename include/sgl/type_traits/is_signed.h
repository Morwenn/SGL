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
#ifndef SGL_TYPE_TRAITS_IS_SIGNED_H_
#define SGL_TYPE_TRAITS_IS_SIGNED_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <sgl/detail/common.h>

#define sgl_is_signed(value)    \
    _Generic( (value),          \
        signed char: true,      \
        signed short: true,     \
        signed int: true,       \
        signed long: true,      \
        signed long long: true, \
        default: false          \
    )

#endif // SGL_TYPE_TRAITS_IS_SIGNED_H_
