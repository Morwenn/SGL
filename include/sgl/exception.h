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
#ifndef SGL_EXCEPTION_H_
#define SGL_EXCEPTION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <setjmp.h>
#include <sgl/detail/common.h>

////////////////////////////////////////////////////////////
// Exceptions

/**
 * @brief Basic exception type
 *
 * The exceptions must be integers to satisfy the longjmp
 * function. The names used here are those of the exceptions
 * defined in the C++ <stdexcept> header and some other ones
 * from the C++ standard library. The mechanism cannot be
 * extended with user-defined types.
 */
typedef enum
{
    logic_error = 1,
    domain_error,
    invalid_argument,
    length_error,
    out_of_range,
    runtime_error,
    range_error,
    overflow_error,
    underflow_error,
    bad_alloc,
    sgl_detail_exceptions_number
} sgl_exception_t;

#ifndef SGL_MAX_EXCEPTIONS

    /**
     * @def SGL_MAX_EXCEPTIONS
     *
     * Maximum number of nested exception blocks allowed in SGL.
     * It can be set to N with the compiler option
     * -DSGL_MAX_EXCEPTIONS=N
     */
    #define SGL_MAX_EXCEPTIONS 32

#endif

////////////////////////////////////////////////////////////
// Global implementation variables

// Global array of jmp_buf
extern jmp_buf sgl_detail_buf_array[SGL_MAX_EXCEPTIONS];

// Whether we are in an exception catch bloc
extern bool sgl_detail_in_catch_bloc[SGL_MAX_EXCEPTIONS];

// Current exception index
extern int sgl_detail_exceptions_index;

// Current exception
extern sgl_exception_t sgl_detail_current_exception;

////////////////////////////////////////////////////////////
// Exception handling functions

/**
 * Throws the given exception.
 */
noreturn void sgl_throw(sgl_exception_t exception);

/**
 * Rethrows the current exception.
 */
noreturn void sgl_rethrow();

/**
 * Returns whether an exception "inherits" from another
 * exception so that it is possible to catch several
 * related exceptions in a single catch block.
 */
bool sgl_exception_inherits_from(sgl_exception_t exception,
                                 sgl_exception_t from);

/**
 * Returns the error message associated to the given
 * exception.
 */
const char* sgl_what(sgl_exception_t exception);

////////////////////////////////////////////////////////////
// Exception handling macros

/**
 * @def sgl_try
 *
 * Beginning of an exception try bloc.
 */
#define sgl_try                                                             \
    do {                                                                    \
        ++sgl_detail_exceptions_index;                                      \
        sgl_detail_in_catch_bloc[sgl_detail_exceptions_index] = false;      \
        if (not setjmp(sgl_detail_buf_array[sgl_detail_exceptions_index]))  \
        {

/**
 * @def sgl_catch(exception)
 *
 * Catches an exception and execute the following bloc
 * of code if it corresponds to the thrown exceptions
 */
#define sgl_catch(exception)                                                            \
        }                                                                               \
        else if (sgl_exception_inherits_from(sgl_detail_current_exception, exception))  \
        {                                                                               \
            sgl_detail_in_catch_bloc[sgl_detail_exceptions_index] = true;               \
            --sgl_detail_exceptions_index;

/**
 * @def sgl_endtry
 *
 * If something was thrown but nothing caught, throws it again.
 * Prints the current exception and aborts with -1 if we are not
 * into a catch block.
 */
#define sgl_endtry                                  \
        }                                           \
        else                                        \
        {                                           \
            if (sgl_detail_exceptions_index > 0)    \
            {                                       \
                --sgl_detail_exceptions_index;      \
                sgl_rethrow();                      \
            }                                       \
            else                                    \
            {                                       \
                sgl_terminate();                    \
            }                                       \
        }                                           \
        --sgl_detail_exceptions_index;              \
    } while (0);

////////////////////////////////////////////////////////////
// Program termination utilities

/**
 * Type of the function called by sgl_terminate. While it does
 * not appear in the typedef, termination handlers should be
 * noreturn functions.
 */
typedef void (*sgl_terminate_handler)();

/**
 * @return the current termination handler.
 */
sgl_terminate_handler sgl_get_terminate();

/**
 * Sets a new termination handler. If \a new_handler is NULL,
 * this functions sets back the default termination handler.
 *
 * @param new_handler New termination handler.
 * @return the old termination handler.
 */
sgl_terminate_handler sgl_set_terminate(sgl_terminate_handler new_handler);

/**
 * Calls the current termination handler. The default one
 * calls abort. This function is called when an exception
 * is thrown and not caught.
 */
noreturn void sgl_terminate();

#endif // SGL_EXCEPTION_H_
