/* assertion for debugging purposes. */
#ifndef YASSERT_H
# define YASSERT_H

#if defined (__GLIBC__)
# define _GNU_SOURCE
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined (__GLIBC__)
/* For glibc. */
extern const char *program_invocation_short_name;
# define __YASSERT_PROGRAM_NAME    program_invocation_short_name
#else
/* Anything else (unix-like system). */
extern const char *__progname;
# define __YASSERT_PROGRAM_NAME    __progname
#endif

#define __yassert_do_internal(arg)					\
	do {								\
		if (!(arg)) {						\
			fprintf(stderr,					\
				"%s: %s:%d: %s: Assertion '"#arg"' failed.\n", \
			        __YASSERT_PROGRAM_NAME, __FILE__, \
				__LINE__, __FUNCTION__);		\
			abort();					\
		}							\
	} while (0)

#define yassert(expr)				\
	__yassert_do_internal(expr)

/* Assert true, if the value is > than 0. */
#define yassert_true(val)				\
	__yassert_do_internal((int32_t)val > 0); }

/* Assert false, if the value is equal to 0. */
#define yassert_false(val)				\
	__yassert_do_internal((int32_t)val == 0)

/* Test whether two i32 values are equal or not. */
#define yassert_i32_eq(left, right)				\
	__yassert_do_internal((int32_t)left == (int32_t)right)

/* Test whether two u32 values are equal or not. */
#define yassert_u32_eq(left, right)					\
	__yassert_do_internal((uint32_t)left == (uint32_t)right)

/* Test whether two i64 values are equal or not. */
#define yassert_i64_eq(left, right)				\
	__yassert_do_internal((int64_t)left == (int64_t)right)

/* Test whether two u64 values are equal or not. */
#define yassert_u64_eq(left, right)					\
	__yassert_do_internal((uint64_t)left == (uint64_t)right)

/* Test whether two f32 values are equal or not. */
#define yassert_f32_eq(left, right)				\
	__yassert_do_internal((float)left == (float)right)

/* Test whether two f64 values are equal or not. */
#define yassert_f64_eq(left, right)				\
	__yassert_do_internal((double)left == (double)right)

/* Test whether two char pointers values are equal or not (case-sensitive). */
#define yassert_cp_case_eq(left, right)			\
	__yassert_do_internal(strcmp(left, right) == 0)

/* Test whether two char pointers values are equal or not (case-insensitive). */
#define yassert_cp_noncase_eq(left, right)			\
	__yassert_do_internal(strcasecmp(left, right) == 0)

/* Test whether two static arrays contains similar values or not. */
#define yassert_static_narr_eq(left, right)				\
	do {								\
		size_t i;						\
	        for (i = 0; i < sizeof(left)/sizeof(*left); i++)	\
			__yassert_do_internal(left[i] == right[i]);	\
	} while (0)

/* Check whether haystack (char pointer) contains a needle (single character). */
#define yassert_c_contains(left, right)				\
	do {							\
		if (strchr(left, right) == NULL)		\
			__yassert_do_internal(left == right);	\
	} while (0)

/* Check whether haystack (char pointer) contains a needle (char pointer). */
#define yassert_cp_contains(left, right)			\
	do {							\
		if (strstr(left, right) == NULL)		\
			__yassert_do_internal(left == right);	\
	} while (0)

#endif /* YASSERT_H */
