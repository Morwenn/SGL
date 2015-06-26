SGL - Standard Generic Library
===

The SGL is an effort to port as many features as possible from the C++ Standard
Template Library (STL) to C11. It includes - among other helpers - iterators,
ranges, algorithms and collections.

The SGL heavily relies on C99 but also makes use of some of the most recent C11
features, such as `_Generic` and `_Static_assert`. Therefore, it is not compatible
with C89 nor C99.

Design & philosophy
---

First of all, it is a C11-only library and is by no means meant to be used with
C++ since C++ already provides better alternatives to equivalent features. The SGL
uses parenthesis for generic parameters where the STL uses chevrons for template
parameters. Every feature of the library is prefixed with `sgl_`.

Also, we don't really like `_Horrible_case` keywords. The following headers are
included by every SGL header by default:

* `<assert.h>`

* `<iso646.h>`

* `<stdbool.h>`

* `<stdalign.h>`

* `<stddef.h>`

Documentation
---

As of now, there is no documentation. There will probably be some Sphinx documentation
at some point in the future.
