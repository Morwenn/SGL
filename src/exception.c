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
#include <stdio.h>
#include <stdlib.h>
#include <sgl/exception.h>

////////////////////////////////////////////////////////////
// Global implementation variables

jmp_buf sgl_detail_buf_array[SGL_MAX_EXCEPTIONS];

bool sgl_detail_in_catch_bloc[SGL_MAX_EXCEPTIONS];

int sgl_detail_exceptions_index = -1;

sgl_exception_t sgl_detail_current_exception;

////////////////////////////////////////////////////////////
// Throwing functions

void sgl_throw(sgl_exception_t exception)
{
    assert(exception != sgl_exception);
    sgl_detail_current_exception = exception;

    // Handle exceptions out of a try block
    if (sgl_detail_exceptions_index == -1)
    {
        sgl_terminate();
    }

    if (sgl_detail_in_catch_bloc[sgl_detail_exceptions_index])
    {
        --sgl_detail_exceptions_index;
    }
    longjmp(sgl_detail_buf_array[sgl_detail_exceptions_index], true);
}

noreturn void sgl_rethrow()
{
    sgl_throw(sgl_detail_current_exception);
}

////////////////////////////////////////////////////////////
// Exceptions inheritance emulation

bool sgl_exception_inherits_from(sgl_exception_t exception,
                                 sgl_exception_t from)
{
    static uint_fast16_t inheritance[] = {
        1 << sgl_logic_error,
        1 << sgl_domain_error | 1 << sgl_logic_error,
        1 << sgl_invalid_argument | 1 << sgl_logic_error,
        1 << sgl_length_error | 1 << sgl_logic_error,
        1 << sgl_out_of_range | 1 << sgl_logic_error,
        1 << sgl_runtime_error,
        1 << sgl_range_error | 1 << sgl_runtime_error,
        1 << sgl_overflow_error | 1 << sgl_runtime_error,
        1 << sgl_underflow_error | 1 << sgl_runtime_error,
        1 << sgl_bad_alloc
    };

    if (from == sgl_exception)
    {
        // Everything inherits from exception
        return true;
    }
    return inheritance[exception] & (1 << from);
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

    if (exception < 0 || exception >= sgl_detail_exceptions_number)
    {
        return "unknown error";
    }
    return messages[exception];
}

////////////////////////////////////////////////////////////
// Program termination utilities

// Default termination function
static noreturn void default_terminate()
{
    printf("Terminate called after throwing an exception.\n");
    printf("  what(): %s\n", sgl_what(sgl_detail_current_exception));
    abort();
}

// Current termination function
static sgl_terminate_handler current_terminate = default_terminate;

sgl_terminate_handler sgl_get_terminate()
{
    return current_terminate;
}

sgl_terminate_handler sgl_set_terminate(sgl_terminate_handler new_handler)
{
    sgl_terminate_handler old_handler = sgl_get_terminate();
    if (new_handler == NULL)
    {
        new_handler = default_terminate;
    }
    current_terminate = new_handler;
    return old_handler;
}

noreturn void sgl_terminate()
{
    current_terminate();
}
