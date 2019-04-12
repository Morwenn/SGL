/*
 * Copyright (C) 2019 Morwenn
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

#ifndef SGL_IO_IOSTREAM_H_
#define SGL_IO_IOSTREAM_H_

#define __STDC_WANT_LIB_EXT2__ 1 // try to get getline
#include <sgl/detail/common.h>
#include <sgl/detail/map.h>
#include <stdio.h>  // getchar, scanf, printf, fprintf, EOF, fgets
#include <stdlib.h> // size_t, malloc, realloc, free
#include <string.h> // memset, strcmp, strlen
#include <stdint.h> // uintptr_t

// Clears the data left over by scanf.
static inline void _sgl_clear_stdin(void)
{
    int _c;
    while ((_c = getchar()) != '\n' && _c != EOF)
        ;
}

/// A wrapper for fgets which also null terminates the string and doesn't
/// leave anything in stdin.
#ifdef __GNUC__
__attribute__((__unused__)) // avoid -Wunused
#endif
static int _sgl_length_getline(char *_x, size_t _length)
{
    if (_x == NULL) {
        return EOF;
    }
    if (_length < 1) {
        fputs("ERROR: sgl_cin: must provide a size for reading to a string buffer.\n", stderr);
        return EOF;
    }

    memset(_x, 0, _length);

    // Skip whitespace and read the first character.
    if (scanf(" %c", _x) == EOF) {
        return EOF;
    }
    if (_length == 2) {
        return 1;
    }
    // Read starting at the first character because we just got it from
    // scanf.
    if (fgets(_x + 1, (int)_length - 1, stdin) == NULL) {
        return EOF;
    }

    // Find the newline in the string. Replace it with '\0' if it exists,
    // or clear out the line because there is data left over.
    char *_newline = strchr(_x, '\n');
    if (_newline != NULL) {
        *_newline = '\0';
    } else {
        // There is extra data, ignore it.
        _sgl_clear_stdin();
    }
    return 1;
}

/// Dynamically allocates a buffer large enough to store the next line from stdin, null terminates it (removing '\n'),
/// and stores it in _x.
#ifdef __GNUC__
__attribute__((__unused__)) // avoid -Wunused
#endif
static int _sgl_malloc_getline(char **_x)
{
#if (defined(__STDC_ALLOC_LIB__) /* C2x hyyype! */ \
        || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200803L) || defined(_GNU_SOURCE) /* glibc */ \
        || defined(_WITH_GETLINE) /* BSD */ \
      )
    if (_x == NULL) {
        return EOF;
    }

    char _tmp;
    // Skip whitespace until the first character.
    if (scanf(" %c", &_tmp) == EOF) {
        return EOF;
    }

    // We have to put that first one back and read again.
    // getline will (re)allocate memory and you can't give an
    // offset pointer to realloc.
    ungetc(_tmp, stdin);

    // Set up *_x for getline
    *_x = NULL;
    size_t _size = 0;

    // Read the line
    if (getline(_x, &_size, stdin) == -1) {
        free(*_x);
        *_x = NULL;
        return EOF;
    }

    // Replace '\n' with '\0'.
    char *_newline = strchr(*_x, '\n');
    if (_newline != NULL) {
        *_newline = '\0';
    }
    return 1;

#else // No getline. :(
    if (_x == NULL) {
        return EOF;
    }
    // 64 should be a decent size.
    int _size = 64;
    int _offset = 1;

    *_x = malloc((size_t)_size + 2);
    if (*_x == NULL) {
        return EOF;
    }
    // Skip whitespace and read the first character
    if (scanf(" %c", *_x) == EOF) {
        free(*_x);
        *_x = NULL;
        return EOF;
    }

    // Read in chunks starting at the first character.
    while (fgets((*_x) + _offset, _size + 1, stdin) != NULL) {
        char *_newline;
        // Find the newline and replace it if we can
        if ((_newline = strchr((*_x) + _offset, '\n')) != NULL) {
            *_newline = '\0';
            return 1;
        } else if (feof(stdin)) {
            // We might be at the end.
            return 1;
        } else {
            // Grow the buffer by _size
            _offset += _size;
            char *_tmp = realloc(*_x, (size_t)(_size + _offset + 1));
            if (_tmp == NULL) {
                free(*_x);
                *_x = NULL;
                return EOF;
            }
            *_x = _tmp;
        }
    }
    // shouldn't be able to get here but ok whatever
    return EOF;
#endif // No getline
}

// Reads either "true" or "false" from stdin like std::boolalpha if _alpha
// is true, or reads an integer and converts to bool.
#ifdef __GNUC__
__attribute__((__unused__)) // avoid -Wunused
#endif
static int _sgl_read_bool(bool *_b, bool _alpha)
{
    if (_b == NULL) {
        return EOF;
    }
    if (_alpha) {
        char _buf[7];
        // We read just enough to get a full word plus a space.
        if (scanf("%6s", _buf) == EOF) {
            return EOF;
        }
        if (strlen(_buf) == 5 && !strcmp(_buf, "false")) {
            *_b = false;
        } else if (strlen(_buf) == 4 && !strcmp(_buf, "true")) {
            *_b = true;
        } else {
            return EOF;
        }
        return 1;
    } else {
        long long val = 0;
        // Read into long long because it is the largest type we can have.
        // Otherwise, truncation might cause issues.
        if (scanf("%lli", &val) < 1) {
            return EOF;
        }

        // "cast to bool"
        *_b = !!val;
        return 1;
    }
}

// Because C sets char literals to int, we need to use some
// hackery to tell the difference from an int and a char literal.
// We can do this by taking the stringified text and checking
// for single quotes.
#define _sgl_is_char_literal(x) (sizeof(#x) >= 4 && ((#x)[0] == '\'' || (#x)[sizeof(#x) - 2] == '\''))

// Generates a format string for printf.
// Good compilers will constant fold this into a literal. GCC needs -O3, Clang needs -O2.
#define _sgl_generate_format_string(x)                                                  \
do {                                                                                    \
   *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = '%';                                      \
   *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = _Generic((x),                             \
        bool: 'd',                                                                      \
        char: 'c',                                                                      \
        signed char: 'h',                                                               \
        unsigned char: 'h',                                                             \
        signed short: 'h',                                                              \
        unsigned short: 'h',                                                            \
        signed int: _sgl_is_char_literal(x) ? 'c' : 'd',                                \
        unsigned int: 'u',                                                              \
        signed long: 'l',                                                               \
        unsigned long: 'l',                                                             \
        long long: 'l',                                                                 \
        unsigned long long: 'l',                                                        \
        float: 'f',                                                                     \
        double: 'l',                                                                    \
        long double: 'L',                                                               \
        char *: 's',                                                                    \
        const char *: 's'                                                               \
    );                                                                                  \
    /* Some of the format strings have more chars to do. Otherwise, we do (void)0. */   \
    _Generic((x),                                                                       \
        bool: (void)0,                                                                  \
        char: (void)0,                                                                  \
        signed char: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ =  'h',                   \
        unsigned char: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'h',                  \
        signed short: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'u',                   \
        unsigned short: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'd',                 \
        signed int: (void)0,                                                            \
        unsigned int: (void)0,                                                          \
        signed long: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'd',                    \
        unsigned long: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'u',                  \
        long long: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'l',                      \
        unsigned long long: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'l',             \
        float: (void)0,                                                                 \
        double: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'f',                         \
        long double: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'f',                    \
        default: (void)0                                                                \
    );                                                                                  \
    _Generic((x),                                                                       \
        bool: (void)0,                                                                  \
        char: (void)0,                                                                  \
        signed char: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'd',                    \
        unsigned char:*_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'u',                   \
        long: (void)0,                                                                  \
        unsigned long: (void)0,                                                         \
        long long: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'd',                      \
        unsigned long long: *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = 'u',             \
        default: (void)0                                                                \
    );                                                                                  \
} while (0);

// Determine the length needed for the format string.
// The trailing plus is intentional. It allows us to put these macros
// together indefinitely, and since we need space for the null terminator,
// we can just put a 1 after the macro.
#define _sgl_fmt_length(x)                                                              \
/* %  */  1 +                                                                           \
          _Generic((x),                                                                 \
/* d   */     bool: 1,                                                                  \
/* c   */     char: 1,                                                                  \
/* hhd */     signed char: 3,                                                           \
/* hhu */     unsigned char: 3,                                                         \
/* hd  */     signed short: 2,                                                          \
/* hu  */     unsigned short: 2,                                                        \
/* d/c */     signed int: 1,                                                            \
/* u   */     unsigned int: 1,                                                          \
/* ld  */     signed long: 2,                                                           \
/* lu  */     unsigned long: 2,                                                         \
/* lld */     signed long long: 3,                                                      \
/* llu */     unsigned long long: 3,                                                    \
/* f   */     float: 1,                                                                 \
/* lf  */     double: 2,                                                                \
/* Lf  */     long double: 2,                                                           \
/* etc */     default: 1                                                                \
          ) +


// your average concat macros
#define _sgl_concat2(x,y) x##y
#define _sgl_concat(x, y) _sgl_concat2(x,y)

/// void sgl_cout(...):
/// Prints one or more items to stdout, using _Generic to deduce the proper types
/// Usage:
///   {int, char, unsigned int, const char *} x = ...;
///   sgl_cout(x, sgl_endl);
///
///   sgl_cout("Hello world");
///
/// Note: Because character literals are int by default, detection of these is a little
/// hacky.
/// If the literal LEXICALLY begins or ends in a single quote (') or is manually cast
/// to char, it will use putchar. As in, the actual macro parameters are stringified and
/// checked.
///
///   sgl_cout('a');          // a
///   sgl_cout(('a'));        // 97
///   sgl_cout((char)('a'));  // a
///   sgl_cout('a' + 1);      // b
///   sgl_cout(1 + 'a');      // b
///   sgl_cout(1 + 'a' + 2);  // 100
///   int c = 'a';
///   sgl_cout(c);            // 97
///   sgl_cout((char)c);      // a
///   char d = 'a';
///   sgl_cout(d);            // a
#define sgl_cout(...)                                                                           \
do {                                                                                            \
    /* Create our string. The 1 is not a typo, see _sgl_fmt_length. */                          \
    char _sgl_concat(_sgl_fmt_buf, __LINE__)[MAP(_sgl_fmt_length, __VA_ARGS__) 1];              \
    /* Create an iterator pointer. */                                                           \
    char *_sgl_concat(_sgl_fmt_buf_p, __LINE__) = _sgl_concat(_sgl_fmt_buf, __LINE__);          \
    /* Generate the format string. With good optimizations, the string will be generated        \
     * at compile time. GCC works with -O3 and Clang with -O2. */                               \
    MAP(_sgl_generate_format_string, __VA_ARGS__);                                              \
    /* Null terminate the string. */                                                            \
    *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = '\0';                                            \
    /* Now print it with printf. */                                                             \
    printf(_sgl_concat(_sgl_fmt_buf,__LINE__), __VA_ARGS__);                                    \
} while (0)

// void sgl_cerr(...);
// Like sgl_cout. but for cerr.
#define sgl_cerr(...)                                                                           \
do {                                                                                            \
    /* Create our string. The 1 is not a typo, see _sgl_fmt_length. */                          \
    char _sgl_concat(_sgl_fmt_buf, __LINE__)[MAP(_sgl_fmt_length, __VA_ARGS__) 1];              \
    /* Create an iterator pointer. */                                                           \
    char *_sgl_concat(_sgl_fmt_buf_p, __LINE__) = _sgl_concat(_sgl_fmt_buf, __LINE__);          \
    /* Generate the format string. With good optimizations, the string will be generated        \
     * at compile time. GCC works with -O3 and Clang with -O2. */                               \
    MAP(_sgl_generate_format_string, __VA_ARGS__);                                              \
    /* Null terminate the string. */                                                            \
    *_sgl_concat(_sgl_fmt_buf_p, __LINE__)++ = '\0';                                            \
    /* Now print it with fprintf. */                                                            \
    fprintf(stderr, _sgl_concat(_sgl_fmt_buf,__LINE__), __VA_ARGS__);                           \
} while (0)

// To make sgl_cin work with strict aliasing.
union _sgl_iostream_pun {
    bool *_b;
    char *_c;
    char **_char_p;
    unsigned char *_hhu;
    signed char *_hhi;
    unsigned short *_hu;
    signed short *_hi;
    unsigned int *_u;
    signed int *_i;
    unsigned long *_lu;
    signed long *_li;
    unsigned long long *_llu;
    signed long long *_lli;
    float *_f;
    double *_lf;
    long double *_Lf;
    void *_p;
};

/// See below.
#define _sgl_cin_impl(x, extra)                                                         \
do {                                                                                    \
    union _sgl_iostream_pun _sgl_concat(_x, __LINE__) = { ._p = &(x) };                 \
    /* Choose the correct format string. */                                             \
    int _sgl_concat(_tmp, __LINE__) = _Generic((x),                                     \
        bool: _sgl_read_bool(_sgl_concat(_x, __LINE__)._b, extra /* boolalpha? */),     \
        char: scanf(" %c", _sgl_concat(_x, __LINE__)._c),                               \
        signed char: scanf("%hhi", _sgl_concat(_x, __LINE__)._hhi),                     \
        unsigned char: scanf("%hhu", _sgl_concat(_x, __LINE__)._hhu),                   \
        signed short: scanf("%hi", _sgl_concat(_x, __LINE__)._hi),                      \
        unsigned short: scanf("%hu", _sgl_concat(_x, __LINE__)._hu),                    \
        signed int: scanf("%i", _sgl_concat(_x, __LINE__)._i),                          \
        unsigned int: scanf("%u", _sgl_concat(_x, __LINE__)._u),                        \
        signed long: scanf("%li", _sgl_concat(_x, __LINE__)._li),                       \
        unsigned long: scanf("%lu", _sgl_concat(_x, __LINE__)._lu),                     \
        signed long long: scanf("%lli", _sgl_concat(_x, __LINE__)._lli),                \
        unsigned long long: scanf("%llu", _sgl_concat(_x, __LINE__)._llu),              \
        float: scanf("%f", _sgl_concat(_x, __LINE__)._f),                               \
        double: scanf("%lf", _sgl_concat(_x, __LINE__)._lf),                            \
        long double: scanf("%Lf", _sgl_concat(_x, __LINE__)._Lf),                       \
        /* uintptr_t hides the "cast to pointer from smaller size" warning */           \
        char **: _sgl_malloc_getline((char **)((uintptr_t)(x))),                        \
        char *: _sgl_length_getline((char *)((uintptr_t)(x)), extra /* bufsize */)      \
    );                                                                                  \
    /* set the value to zero on errors like std::cin. */                                \
    if (_sgl_concat(_tmp, __LINE__) < 1) {                                              \
        _Generic((x),                                                                   \
            char *: (void)0, char **: (void)0,                                          \
            default: (void)memset(_sgl_concat(_x, __LINE__)._p, 0, sizeof(x))           \
        );                                                                              \
    }                                                                                   \
    /* Don't clear when we have char * or char ** as those do that automatically. */    \
    _Generic((x), char *: (void)0, char **: (void)0, default: _sgl_clear_stdin());      \
} while (0)

/// Wrapper for sgl_cin that prevents variadic macro warnings and sets extra to zero if needed.
#define _sgl_cin_impl_wrap(x, extra, ...) _sgl_cin_impl(x, extra + 0)

/// void sgl_cin(T x, [size_t len|bool boolalpha]):
/// Reads a line from stdin into x using _Generic to determine the type.
/// For char *, the len argument is required and it sets the limit to how long the input
/// will be including a null terminator.
/// For both char * and char **, the string will be null terminated, with any newlines
/// removed.
///
/// Usage:
///   // Basic arithmetic types can be read like so:
///   {bool, int, char, signed char, unsigned long long, float, etc} x;
///   sgl_cin(x);
///
///   // Truncated strings pass the length (including the null terminator) as a second argument.
///   char str[21];
///   sgl_cin(str, sizeof(str));
///
///   // A dynamically allocated line will be recieved like so
///   char *buf = NULL;
///   char **tmp = &buf;
///   sgl_cin(tmp);
///   // don't forget to free!
///
/// Caveats:
///  - To read a character, use 'char'. To read an 8-bit signed integer, use 'signed char',
///    and for an 8-bit unsigned integer, use 'unsigned char'.
///  - Hex and octal are only detected with signed variables, due to the limitations of
///    scanf.
///  - As shown above, you need a temporary variable set to the address of a null char
///    pointer to get the automatic memory allocation to work. The buffer must be freed.
///  - sgl_cin isn't going to let you buffer overflow on strings. Give it a buffer size or
///    allocate.
///  - Performance is eh on this one. Since we have to use custom routines for strings and
///    bools, we can't just generate a scanf string and call it a day.
#define sgl_cin(...) _sgl_cin_impl_wrap(__VA_ARGS__, 0, 0)

// To be provided as a second argument for sgl_cin, which makes it so it reads "true" or "false"
#define sgl_boolalpha 1
#define sgl_endl (char)'\n'

#endif // SGL_IO_IOSTREAM_H_

