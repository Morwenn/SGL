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
#include <sgl/type_traits.h>

int main()
{
    static_assert(sgl_is_floating_point(2.0f), "");
    static_assert(sgl_is_floating_point(2.0), "");
    static_assert(sgl_is_floating_point(2.0l), "");
    static_assert(not sgl_is_floating_point(2), "");

    static_assert(sgl_is_instance(2.0f, float), "");
    static_assert(sgl_is_instance(2.0, double), "");
    static_assert(sgl_is_instance(NULL, void*), "");
    static_assert(sgl_is_instance(2ull, unsigned long long), "");
    static_assert(sgl_is_instance(sizeof(int), size_t), "");

    static_assert(sgl_is_same(int, int), "");
    static_assert(not sgl_is_same(int, unsigned), "");

    static_assert(sgl_is_signed(2), "");
    static_assert(sgl_is_signed(2l), "");
    static_assert(sgl_is_signed(2ll), "");

    static_assert(sgl_is_unsigned(2u), "");
    static_assert(sgl_is_unsigned(2ul), "");
    static_assert(sgl_is_unsigned(2ull), "");
}
