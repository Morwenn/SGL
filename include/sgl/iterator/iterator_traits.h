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
#ifndef SGL_ITERATOR_ITERATOR_TRAITS_H_
#define SGL_ITERATOR_ITERATOR_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <sgl/detail/common.h>
#include <sgl/utility/paste.h>

#define sgl_value_type(type) \
        sgl_paste(type, _value_type)

#define sgl_size_type(type) \
        sgl_paste(type, _size_type)

#define sgl_difference_type(type) \
        sgl_paste(type, _difference_type)

#define sgl_iterator(type) \
        sgl_paste(type, _iterator)

#define sgl_const_iterator(type) \
        sgl_paste(type, _const_iterator)

#endif // SGL_ITERATOR_ITERATOR_TRAITS_H_
