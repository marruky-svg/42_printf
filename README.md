*This project has been created as part of the 42 curriculum by elsouza.*

# ft_printf

## Description

`ft_printf` is a reimplementation of the C standard library function `printf()`, packaged as a static library (`libftprintf.a`). The main goal of the project is to learn how **variadic functions** work in C (`va_start`, `va_arg`, `va_end`), and to design a small, well-structured and extensible formatting engine on top of the `write()` system call.

The function has the same prototype and return value as the original:

```c
int ft_printf(const char *format, ...);
```

It returns the number of characters printed, or `-1` if a write error occurs.

### Supported conversions

| Conversion | Output |
|---|---|
| `%c` | A single character |
| `%s` | A string (`(null)` if the pointer is `NULL`) |
| `%p` | A `void *` pointer in hexadecimal, prefixed with `0x` |
| `%d` / `%i` | A signed decimal integer |
| `%u` | An unsigned decimal integer |
| `%x` / `%X` | An unsigned integer in lowercase / uppercase hexadecimal |
| `%%` | A literal percent sign |

Only the mandatory part is implemented (no flags, field width or precision).

## Instructions

### Compilation

```sh
make        # builds libftprintf.a at the root of the repository
make clean  # removes object files
make fclean # removes object files and the library
make re     # rebuilds everything from scratch
```

The library is compiled with `cc -Wall -Wextra -Werror` and created with `ar rcs`.

### Usage

Include the header and link against the library:

```c
#include "ft_printf.h"

int main(void)
{
	int	n;

	n = ft_printf("Hello %s, you are %d years old (%x in hex)\n", "Elder", 23, 23);
	ft_printf("Printed %d characters\n", n);
	return (0);
}
```

```sh
cc -Wall -Wextra -Werror main.c -L. -lftprintf -o test && ./test
```

`-L.` tells the linker to look for libraries in the current directory, and `-lftprintf` resolves to `libftprintf.a`.

## Algorithm and data structures

### Overall flow: a single pass over the format string

`ft_printf` walks the format string once, from left to right, using an index `i`:

1. If the current character is not `%`, it is written as-is.
2. If it is `%` followed by another character, that character is the conversion specifier. It is passed to a dispatch function together with the argument list, and `i` advances by two.
3. Every helper returns the number of characters it wrote, which is added to a running total. If any helper returns `-1`, `ft_printf` stops, calls `va_end` and returns `-1`.

This is an O(n) single pass with no look-ahead beyond one character, which is all the mandatory conversions need.

**Why this approach:** the mandatory part has no flags or widths, so every conversion can be printed the moment it is read. Building an intermediate string would add `malloc`/`free` and error handling without any benefit. The trade-off is that this design does not extend directly to field widths (where the length of the output must be known before printing the padding). That is acceptable because the bonus part was not attempted.

### Dispatch function (`convertion_handler`)

A single function maps each specifier to the helper that prints it and pulls the argument of the correct type with `va_arg`. Adding a new conversion means adding one branch. For example, octal output would be one line reusing the generic number printer with the alphabet `"01234567"` and base 8.

The `va_list` is passed **by pointer** (`va_list *ap`) rather than by value. In C, if a `va_list` is passed by value to another function that calls `va_arg` on it, the caller's copy becomes indeterminate. Passing a pointer guarantees that every helper consumes arguments from the same, consistently advancing list.

### Generic base conversion (`print_num`)

All unsigned numeric conversions (`%u`, `%x`, `%X`, and the digits of `%p`, `%d` and `%i`) go through a single recursive function:

```c
int print_num(unsigned long n, const char *set, unsigned long len);
```

- `set` is the **digit alphabet** used as a lookup table (`"0123456789"`, `"0123456789abcdef"`, `"0123456789ABCDEF"`). The digit for a value `d` is simply `set[d]`.
- `len` is the base.
- The function first recurses on `n / len`, then prints `set[n % len]`. Because the recursive call happens *before* the write, the most significant digit is printed first, with no need to store digits in a temporary buffer and reverse them.

Example for `print_num(255, "0123456789abcdef", 16)`:

| Call | `n / 16` | `n % 16` | Prints |
|---|---|---|---|
| `print_num(255)` | 15 → recurse | 15 | `f` (second) |
| `print_num(15)` | 0 → stop | 15 | `f` (first) |

Output: `ff`.

The recursion depth is bounded by the number of digits: at most 20 levels for a 64-bit value in base 10 and 16 levels in base 16, so stack usage is negligible.

**Why `unsigned long`:** it is wide enough to hold both an `unsigned int` (for `%u`/`%x`/`%X`) and a pointer value (for `%p`) on the 64-bit platforms used at 42, so one function covers every case.

### Signed numbers (`print_signed`)

The `int` argument is first copied into a `long`. This makes it safe to negate `INT_MIN` (`-2147483648`), whose positive value does not fit in an `int`. If the number is negative, a `-` is printed and the absolute value is passed to `print_num`.

### Pointers (`print_ptr`)

Prints the `0x` prefix, then the address in lowercase hexadecimal through `print_num`.

How a `NULL` pointer is printed is not defined by the C standard, and each libc handles it differently: glibc on Linux prints `(nil)` and the macOS libc prints `0x0`. Since `ft_printf` must match the original `printf()`, the string is chosen at compile time in `ft_printf.h`:

```c
# ifdef __linux__
#  define PTR_NULL "(nil)"
# else
#  define PTR_NULL "0x0"
# endif
```

`__linux__` is predefined by the compiler when targeting Linux, so the choice is made by the preprocessor and costs nothing at runtime. The output and the return value (5 or 3) match the native `printf()` on both platforms.

### Error handling

`write()` returns `-1` on failure. Each helper checks the return value of every write and propagates `-1` upwards, so `ft_printf` returns `-1` just like the original.

### Memory

The implementation performs no heap allocation, so memory leaks are impossible. As required by the subject, the internal buffer management of the original `printf()` is not reproduced: each character is written directly with `write()`. This is simpler but slower, since it makes one system call per character.

### Undefined behavior

The C standard leaves some inputs undefined, for example a lone `%` at the end of the format string or an unknown specifier such as `%k`. `ft_printf` does not crash on these inputs, but its output is not guaranteed to match any particular libc.

## Resources

- [`printf(3)` — Linux manual page](https://man7.org/linux/man-pages/man3/printf.3.html)
- [`stdarg(3)` — Linux manual page](https://man7.org/linux/man-pages/man3/stdarg.3.html)
- [cppreference — `printf`, `fprintf`, ...](https://en.cppreference.com/w/c/io/fprintf)
- [cppreference — Variadic functions](https://en.cppreference.com/w/c/variadic)
- `man 2 write`

### Use of AI

AI (Claude, by Anthropic) was used as a support tool, not to write the implementation:

- **Code:** all source files (`ft_printf.c`, `ft_printf_utils.c`, `ft_printf.h`) and the `Makefile` were written by me. AI was used to review the finished code and to compare its output against the original `printf()` on edge cases (`INT_MIN`, `UINT_MAX`, `NULL` strings and pointers, the `'\0'` character).
- **Concepts:** AI was used to discuss how variadic functions work and the platform-dependent behavior of `%p` with `NULL`.
- **Documentation:** this README was drafted with AI from my code and then reviewed by me.
