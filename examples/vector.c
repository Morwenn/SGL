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
#include <assert.h>
#include <stdio.h>
#include <sgl/exception.h>
#include <sgl/iterator.h>
#include <sgl/utility.h>
#include <sgl/vector.h>

sgl_define(sgl_vector(float))
sgl_define(sgl_vector(int))
sgl_define(sgl_vector(double))

int main()
{
    sgl_vector(int)* vec_i = sgl_new(sgl_vector(int));
    sgl_vector(double)* vec_d = sgl_new(sgl_vector(double));

    printf("is_empty: %d\n", sgl_is_empty(vec_i));
    printf("size: %u\n", sgl_size(vec_i));
    printf("max_size: %u\n", sgl_max_size(vec_i));
    printf("capacity: %u\n", sgl_capacity(vec_i));
    printf("front: %d == %d\n", sgl_front(vec_i), sgl_at(vec_i, 0));
    printf("back: %d == %d\n", sgl_back(vec_i), sgl_at(vec_i, 2));

    sgl_reserve(vec_d, 56);
    printf("capacity: %d\n", sgl_capacity(vec_d)); // 56
    sgl_reserve(vec_d, 30);
    printf("capacity: %d\n", sgl_capacity(vec_d)); // 56

    for (int i = 0 ; i < 3 ; ++i)
    {
        sgl_push_back(vec_d, 0.0);
    }
    sgl_front(vec_d) = 5.6;
    sgl_at(vec_d, 1) = 1.0;
    sgl_at(vec_d, 2) = 2.0;
    for (size_t i = 0 ; i < 3 ; ++i)
    {
        printf("%f ", sgl_at(vec_d, i));
    }
    printf("\n");

    sgl_shrink_to_fit(vec_d);
    printf("capacity: %d\n", sgl_capacity(vec_d)); // 3

    assert(sgl_data(vec_d) == vec_d->_data);
    assert(sgl_back(vec_d) == sgl_at(vec_d, 2));

    for (int i = 5 ; i > 0 ; --i)
    {
        sgl_push_back(vec_i, i);
    }
    printf("size: %u\n", sgl_size(vec_i)); // 5
    printf("capacity: %d\n", sgl_capacity(vec_i)); // 40
    sgl_pop_back(vec_i);
    sgl_pop_back(vec_i);
    sgl_shrink_to_fit(vec_i);
    printf("size: %u\n", sgl_size(vec_i)); // 3
    printf("capacity: %d\n", sgl_capacity(vec_i)); // 3
    int foo = 8;
    sgl_push_back(vec_i, foo);
    printf("size: %u\n", sgl_size(vec_i)); // 4
    printf("capacity: %d\n", sgl_capacity(vec_i)); // 43

    sgl_delete(vec_i);
    sgl_delete(vec_d);

    // erase methods (overloading test)
    sgl_vector(int)* vi = sgl_new(sgl_vector(int));
    for (int i = 0 ; i < 10 ; ++i)
    {
        sgl_push_back(vi, i);
    }

    printf("\n");
    for (sgl_iterator(sgl_vector(int)) it = sgl_begin(vi) ; it != sgl_end(vi) ; ++it)
    {
        // 0 1 2 3 4 5 6 7 8 9
        printf("%d ", *it);
    }

    printf("\n");
    sgl_erase(vi, sgl_begin(vi));
    for (size_t i = 0 ; i < sgl_size(vi) ; ++i)
    {
        // 1 2 3 4 5 6 7 8 9
        printf("%d ", sgl_at(vi, i));
    }

    printf("\n");
    sgl_erase(vi, sgl_begin(vi)+2, sgl_begin(vi)+5);
    for (sgl_size_type(sgl_vector(int)) i = 0 ; i < sgl_size(vi) ; ++i)
    {
        // 1 2 6 7 8 9
        printf("%d ", sgl_at(vi, i));
    }

    sgl_delete(vi);

    // resize methods (overloading test)
    sgl_vector(float)* vf = sgl_new(sgl_vector(float));

    printf("\n");
    sgl_resize(vf, 5);
    for (size_t i = 0 ; i < sgl_size(vf) ; ++i)
    {
        printf("%f ", sgl_at(vf, i));
    }
    sgl_resize(vf, 3);
    printf("\n%d\n", sgl_size(vf));

    sgl_resize(vf, 8, 2.5);
    for (size_t i = 0 ; i < sgl_size(vf) ; ++i)
    {
        printf("%f ", sgl_at(vf, i));
    }

    sgl_delete(vf);

    // Operators tests
    sgl_vector(int)* vec1 = sgl_new(sgl_vector(int));
    sgl_vector(int)* vec2 = sgl_new(sgl_vector(int));

    sgl_push_back(vec1, 2);
    sgl_push_back(vec1, 3);
    sgl_push_back(vec1, 4);
    sgl_push_back(vec1, 5);

    sgl_push_back(vec2, 2);
    sgl_push_back(vec2, 3);
    sgl_push_back(vec2, 4);
    sgl_push_back(vec2, 5);

    sgl_try
    {
        sgl_try
        {
            sgl_throw(out_of_range);
        }
        sgl_catch (logic_error)
        {
            printf("\nCaught exception: logic error\n");
        }
        sgl_endtry
    }
    sgl_catch (out_of_range)
    {
        printf("\nCaught exception: out of range error\n");
    }
    sgl_endtry

    sgl_delete(vec1);
    sgl_delete(vec2);
}
