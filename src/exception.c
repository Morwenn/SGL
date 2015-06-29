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
#include <stdint.h>
#include <sgl/exception.h>

////////////////////////////////////////////////////////////
// Global implementation variables

jmp_buf sgl_detail_buf_array[SGL_MAX_EXCEPTIONS];

bool sgl_detail_in_catch_bloc[SGL_MAX_EXCEPTIONS];

int sgl_detail_exceptions_index = -1;

sgl_exception_t sgl_detail_current_exception;

////////////////////////////////////////////////////////////
// Throwing function

void sgl_throw(sgl_exception_t exception)
{
    if (sgl_detail_in_catch_bloc[sgl_detail_exceptions_index])
    {
        --sgl_detail_exceptions_index;
    }
    sgl_detail_current_exception = exception;
    longjmp(sgl_detail_buf_array[sgl_detail_exceptions_index], true);
}

////////////////////////////////////////////////////////////
// Exceptions inheritance emulation

bool sgl_exception_inherits_from(sgl_exception_t exception,
                                 sgl_exception_t from)
{
    static uint_fast16_t inheritance[] = {
        1 << logic_error,
        1 << domain_error | 1 << logic_error,
        1 << invalid_argument | 1 << logic_error,
        1 << length_error | 1 << logic_error,
        1 << out_of_range | 1 << logic_error,
        1 << runtime_error,
        1 << range_error | 1 << runtime_error,
        1 << overflow_error | 1 << runtime_error,
        1 << underflow_error | 1 << runtime_error,
        1 << bad_alloc
    };

    return inheritance[exception - 1] & (1 << from);
}

////////////////////////////////////////////////////////////
// Exception message function

const char* sgl_what(sgl_exception_t exception)
{
    static const char* const messages[] = {
        "logic error",
        "domain error",
        "invalid argument",
        "length error",
        "out of range error",
        "runtime error",
        "range error",
        "overflow error",
        "underflow error",
        "bad allocation"
    };

    if (exception < 1 || exception >= sgl_detail_exceptions_number)
    {
        return "unknown error";
    }

    // The error code 0 is reserved for when no exception
    // occurs, therefore, we have to subtract 1
    return messages[exception - 1];
}
