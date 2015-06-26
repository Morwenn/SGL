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
#ifndef SGL_UTILITY_PASTE_H_
#define SGL_UTILITY_PASTE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <sgl/detail/common.h>

// Note: some compilers need one more level of indirection
//       than GCC, hence sql_paste3.
#define sgl_paste3(x, y) \
    x ## y

#define sgl_paste2(x, y) \
    sgl_paste3(x, y)

#define sgl_paste1(x, y) \
    sgl_paste2(x, y)

/**
 * @def sgl_paste(x, y)
 *
 * Pastes two macros tokens together to create a new token.
 */
#define sgl_paste(x, y) \
    sgl_paste1(x, y)

#endif // SGL_UTILITY_PASTE_H_
