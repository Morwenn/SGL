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
#ifndef SGL_UTILITY_DEFINE_H_
#define SGL_UTILITY_DEFINE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <sgl/detail/common.h>

/**
 * @def sgl_define(type)
 *
 * Defines all the functions for the given type, provided the
 * type has a confirming definition macro.
 */
#define sgl_define(type) \
    sgl_define_##type

#endif // SGL_UTILITY_DEFINE_H_
