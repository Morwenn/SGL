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
#ifndef SGL_COLLECTION_ERASE_H_
#define SGL_COLLECTION_ERASE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <sgl/detail/common.h>
#include <sgl/utility/dispatch.h>

#define sgl_erase(collection, ...) \
    sgl_dispatch(sgl_erase, __VA_ARGS__)(collection, __VA_ARGS__)

#define sgl_erase1(collection, pos) \
    (collection)->_functions->erase1(collection, pos)

#define sgl_erase2(collection, first, last) \
    (collection)->_functions->erase2(collection, first, last)

#endif // SGL_COLLECTION_ERASE_H_
