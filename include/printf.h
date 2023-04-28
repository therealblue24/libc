#ifndef PRINTF_H_
#define PRINTF_H_

#include <stdarg.h>
#include <stddef.h>
#include <detail/putchar.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#if((__GNUC__ == 4 && __GNUC_MINOR__ >= 4) || __GNUC__ > 4)
#define ATTR_PRINTF(one_based_format_index, first_arg) \
    __attribute__((format(gnu_printf, (one_based_format_index), (first_arg))))
#else
#define ATTR_PRINTF(one_based_format_index, first_arg) \
    __attribute__((format(printf, (one_based_format_index), (first_arg))))
#endif
#define ATTR_VPRINTF(one_based_format_index) \
    ATTR_PRINTF((one_based_format_index), 0)
#else
#define ATTR_PRINTF(one_based_format_index, first_arg)
#define ATTR_VPRINTF(one_based_format_index)
#endif

#ifndef PRINTF_ALIAS_STANDARD_FUNCTION_NAMES
#define PRINTF_ALIAS_STANDARD_FUNCTION_NAMES 0
#endif

#ifndef PRINTF_VISIBILITY
#define PRINTF_VISIBILITY
#endif

PRINTF_VISIBILITY int printf(const char *format, ...) ATTR_PRINTF(1, 2);
PRINTF_VISIBILITY int vprintf(const char *format, va_list arg) ATTR_VPRINTF(1);
PRINTF_VISIBILITY int sprintf(char *s, const char *format, ...)
    ATTR_PRINTF(2, 3);
PRINTF_VISIBILITY int vsprintf(char *s, const char *format, va_list arg)
    ATTR_VPRINTF(2);
PRINTF_VISIBILITY int snprintf(char *s, size_t count, const char *format, ...)
    ATTR_PRINTF(3, 4);
PRINTF_VISIBILITY int vsnprintf(char *s, size_t count, const char *format,
                                va_list arg) ATTR_VPRINTF(3);
PRINTF_VISIBILITY int fctprintf(void (*out)(char c, void *extra_arg),
                                void *extra_arg, const char *format, ...)
    ATTR_PRINTF(3, 4);
PRINTF_VISIBILITY int vfctprintf(void (*out)(char c, void *extra_arg),
                                 void *extra_arg, const char *format,
                                 va_list arg) ATTR_VPRINTF(3);
#ifdef __cplusplus
}
#endif

#endif /* PRINTF_H_ */
