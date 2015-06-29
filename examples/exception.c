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

int main()
{
    sgl_try
    {
        sgl_try
        {
            sgl_throw(out_of_range);
        }
        sgl_catch (logic_error)
        {
            printf("Caught exception: %s\n", sgl_what(logic_error));
            sgl_rethrow();
        }
        sgl_endtry
    }
    sgl_catch (domain_error)
    {
        // Should not be executed
        printf("Caught exception: %s\n", sgl_what(domain_error));
    }
    sgl_catch (out_of_range)
    {
        printf("Caught exception: %s\n", sgl_what(out_of_range));
        sgl_try
        {
            // Should leave the try-catch blocks and cause an error
            sgl_throw(bad_alloc);
        }
        sgl_catch(runtime_error)
        {
            // Should not be executed
            printf("Caught exception: %s\n", sgl_what(runtime_error));
        }
        sgl_endtry
    }
    sgl_endtry
}
