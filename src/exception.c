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
#include <sgl/exception.h>

////////////////////////////////////////////////////////////
// Global implementation variables

jmp_buf sgl_detail_buf_array[SGL_MAX_EXCEPTIONS];

bool sgl_detail_in_catch_bloc[SGL_MAX_EXCEPTIONS];

int sgl_detail_exceptions_index = -1;

////////////////////////////////////////////////////////////
// Throwing function

void sgl_throw(sgl_exception_t exception)
{
    if (sgl_detail_in_catch_bloc[sgl_detail_exceptions_index])
    {
        --sgl_detail_exceptions_index;
    }
    longjmp(sgl_detail_buf_array[sgl_detail_exceptions_index], exception);
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

    // There is no message for no_exception, hence
    // the subtraction
    return messages[exception - 1];
}
