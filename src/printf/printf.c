#include <printf.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>

#ifndef PRINTF_INTEGER_BUFFER_SIZE
#define PRINTF_INTEGER_BUFFER_SIZE 32
#endif

#ifndef PRINTF_DECIMAL_BUFFER_SIZE
#define PRINTF_DECIMAL_BUFFER_SIZE 32
#endif

#ifndef PRINTF_SUPPORT_DECIMAL_SPECIFIERS
#define PRINTF_SUPPORT_DECIMAL_SPECIFIERS 1
#endif

#ifndef PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS
#define PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS 1
#endif

#ifndef PRINTF_SUPPORT_WRITEBACK_SPECIFIER
#define PRINTF_SUPPORT_WRITEBACK_SPECIFIER 1
#endif

#ifndef PRINTF_DEFAULT_FLOAT_PRECISION
#define PRINTF_DEFAULT_FLOAT_PRECISION 6
#endif

#ifndef PRINTF_MAX_INTEGRAL_DIGITS_FOR_DECIMAL
#define PRINTF_MAX_INTEGRAL_DIGITS_FOR_DECIMAL 9
#endif

#ifndef PRINTF_SUPPORT_LONG_LONG
#define PRINTF_SUPPORT_LONG_LONG 1
#endif

#ifndef PRINTF_LOG10_TAYLOR_TERMS
#define PRINTF_LOG10_TAYLOR_TERMS 4
#endif

#if PRINTF_LOG10_TAYLOR_TERMS <= 1
#error \
	"At least one non-constant Taylor expansion is necessary for the log10() calculation"
#endif

#ifndef PRINTF_CHECK_FOR_NUL_IN_FORMAT_SPECIFIER
#define PRINTF_CHECK_FOR_NUL_IN_FORMAT_SPECIFIER 1
#endif

#define PRINTF_PREFER_DECIMAL false
#define PRINTF_PREFER_EXPONENTIAL true

#define PRINTF_CONCATENATE(s1, s2) s1##s2
#define PRINTF_EXPAND_THEN_CONCATENATE(s1, s2) PRINTF_CONCATENATE(s1, s2)
#define PRINTF_FLOAT_NOTATION_THRESHOLD \
	PRINTF_EXPAND_THEN_CONCATENATE(1e, PRINTF_MAX_INTEGRAL_DIGITS_FOR_DECIMAL)

#define FLAGS_ZEROPAD (1U << 0U)
#define FLAGS_LEFT (1U << 1U)
#define FLAGS_PLUS (1U << 2U)
#define FLAGS_SPACE (1U << 3U)
#define FLAGS_HASH (1U << 4U)
#define FLAGS_UPPERCASE (1U << 5U)
#define FLAGS_CHAR (1U << 6U)
#define FLAGS_SHORT (1U << 7U)
#define FLAGS_INT (1U << 8U)
#define FLAGS_LONG (1U << 9U)
#define FLAGS_LONG_LONG (1U << 10U)
#define FLAGS_PRECISION (1U << 11U)
#define FLAGS_ADAPT_EXP (1U << 12U)
#define FLAGS_POINTER (1U << 13U)
#define FLAGS_SIGNED (1U << 14U)

#ifdef PRINTF_SUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS

#define FLAGS_INT8 FLAGS_CHAR

#if(SHRT_MAX == 32767LL)
#define FLAGS_INT16 FLAGS_SHORT
#elif(INT_MAX == 32767LL)
#define FLAGS_INT16 FLAGS_INT
#elif(LONG_MAX == 32767LL)
#define FLAGS_INT16 FLAGS_LONG
#elif(LLONG_MAX == 32767LL)
#define FLAGS_INT16 FLAGS_LONG_LONG
#else
#error "No basic integer type has a size of 16 bits exactly"
#endif

#if(SHRT_MAX == 2147483647LL)
#define FLAGS_INT32 FLAGS_SHORT
#elif(INT_MAX == 2147483647LL)
#define FLAGS_INT32 FLAGS_INT
#elif(LONG_MAX == 2147483647LL)
#define FLAGS_INT32 FLAGS_LONG
#elif(LLONG_MAX == 2147483647LL)
#define FLAGS_INT32 FLAGS_LONG_LONG
#else
#error "No basic integer type has a size of 32 bits exactly"
#endif

#if(SHRT_MAX == 9223372036854775807LL)
#define FLAGS_INT64 FLAGS_SHORT
#elif(INT_MAX == 9223372036854775807LL)
#define FLAGS_INT64 FLAGS_INT
#elif(LONG_MAX == 9223372036854775807LL)
#define FLAGS_INT64 FLAGS_LONG
#elif(LLONG_MAX == 9223372036854775807LL)
#define FLAGS_INT64 FLAGS_LONG_LONG
#else
#error "No basic integer type has a size of 64 bits exactly"
#endif

#endif

typedef unsigned int printf_flags_t;

#define BASE_BINARY 2
#define BASE_OCTAL 8
#define BASE_DECIMAL 10
#define BASE_HEX 16

typedef uint8_t numeric_base_t;

#if PRINTF_SUPPORT_LONG_LONG
typedef unsigned long long printf_unsigned_value_t;
typedef long long printf_signed_value_t;
#else
typedef unsigned long printf_unsigned_value_t;
typedef long printf_signed_value_t;
#endif

typedef unsigned int printf_size_t;
#define PRINTF_MAX_POSSIBLE_BUFFER_SIZE INT_MAX

#if(PRINTF_SUPPORT_DECIMAL_SPECIFIERS || PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS)
#include <float.h>
#if FLT_RADIX != 2
#error "Non-binary-radix floating-point types are unsupported."
#endif

#if DBL_MANT_DIG == 24

#define DOUBLE_SIZE_IN_BITS 32
typedef uint32_t double_uint_t;
#define DOUBLE_EXPONENT_MASK 0xFFU
#define DOUBLE_BASE_EXPONENT 127
#define DOUBLE_MAX_SUBNORMAL_EXPONENT_OF_10 -38
#define DOUBLE_MAX_SUBNORMAL_POWER_OF_10 1e-38

#elif DBL_MANT_DIG == 53

#define DOUBLE_SIZE_IN_BITS 64
typedef uint64_t double_uint_t;
#define DOUBLE_EXPONENT_MASK 0x7FFU
#define DOUBLE_BASE_EXPONENT 1023
#define DOUBLE_MAX_SUBNORMAL_EXPONENT_OF_10 -308
#define DOUBLE_MAX_SUBNORMAL_POWER_OF_10 1e-308

#else
#error "Unsupported double type configuration"
#endif
#define DOUBLE_STORED_MANTISSA_BITS (DBL_MANT_DIG - 1)

typedef union {
	double_uint_t U;
	double F;
} double_with_bit_access;

static inline double_with_bit_access get_bit_access(double x)
{
	double_with_bit_access dwba;
	dwba.F = x;
	return dwba;
}

static inline int get_sign_bit(double x)
{
	return (int)(get_bit_access(x).U >> (DOUBLE_SIZE_IN_BITS - 1));
}

static inline int get_exp2(double_with_bit_access x)
{
	return (int)((x.U >> DOUBLE_STORED_MANTISSA_BITS) & DOUBLE_EXPONENT_MASK) -
		   DOUBLE_BASE_EXPONENT;
}
#define PRINTF_ABS(_x) ((_x) > 0 ? (_x) : -(_x))

#endif

#define ABS_FOR_PRINTING(_x) \
	((printf_unsigned_value_t)((_x) > 0 ? (_x) : -((printf_signed_value_t)_x)))

typedef struct {
	void (*function)(char c, void *extra_arg);
	void *extra_function_arg;
	char *buffer;
	printf_size_t pos;
	printf_size_t max_chars;
} output_gadget_t;

static inline void putchar_via_gadget(output_gadget_t *gadget, char c)
{
	printf_size_t write_pos = gadget->pos++;
	if(write_pos >= gadget->max_chars) {
		return;
	}
	if(gadget->function != NULL) {
		gadget->function(c, gadget->extra_function_arg);
	} else {
		gadget->buffer[write_pos] = c;
	}
}

static inline void append_termination_with_gadget(output_gadget_t *gadget)
{
	if(gadget->function != NULL || gadget->max_chars == 0) {
		return;
	}
	if(gadget->buffer == NULL) {
		return;
	}
	printf_size_t null_char_pos =
		gadget->pos < gadget->max_chars ? gadget->pos : gadget->max_chars - 1;
	gadget->buffer[null_char_pos] = '\0';
}

static inline void putchar_wrapper(char c, void *unused)
{
	(void)unused;
	putchar(c);
}

static inline output_gadget_t discarding_gadget(void)
{
	output_gadget_t gadget;
	gadget.function = NULL;
	gadget.extra_function_arg = NULL;
	gadget.buffer = NULL;
	gadget.pos = 0;
	gadget.max_chars = 0;
	return gadget;
}

static inline output_gadget_t buffer_gadget(char *buffer, size_t buffer_size)
{
	printf_size_t usable_buffer_size =
		(buffer_size > PRINTF_MAX_POSSIBLE_BUFFER_SIZE) ?
			PRINTF_MAX_POSSIBLE_BUFFER_SIZE :
			(printf_size_t)buffer_size;
	output_gadget_t result = discarding_gadget();
	if(buffer != NULL) {
		result.buffer = buffer;
		result.max_chars = usable_buffer_size;
	}
	return result;
}

static inline output_gadget_t function_gadget(void (*function)(char, void *),
											  void *extra_arg)
{
	output_gadget_t result = discarding_gadget();
	result.function = function;
	result.extra_function_arg = extra_arg;
	result.max_chars = PRINTF_MAX_POSSIBLE_BUFFER_SIZE;
	return result;
}

static inline output_gadget_t extern_putchar_gadget(void)
{
	return function_gadget(putchar_wrapper, NULL);
}

static inline printf_size_t strnlen_s_(const char *str, printf_size_t maxsize)
{
	const char *s;
	for(s = str; *s && maxsize--; ++s)
		;
	return (printf_size_t)(s - str);
}

static inline bool is_digit_(char ch)
{
	return (ch >= '0') && (ch <= '9');
}

static printf_size_t atou_(const char **str)
{
	printf_size_t i = 0U;
	while(is_digit_(**str)) {
		i = i * 10U + (printf_size_t)(*((*str)++) - '0');
	}
	return i;
}

static void out_rev_(output_gadget_t *output, const char *buf,
					 printf_size_t len, printf_size_t width,
					 printf_flags_t flags)
{
	const printf_size_t start_pos = output->pos;

	if(!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
		for(printf_size_t i = len; i < width; i++) {
			putchar_via_gadget(output, ' ');
		}
	}

	while(len) {
		putchar_via_gadget(output, buf[--len]);
	}

	if(flags & FLAGS_LEFT) {
		while(output->pos - start_pos < width) {
			putchar_via_gadget(output, ' ');
		}
	}
}

static void print_integer_finalization(output_gadget_t *output, char *buf,
									   printf_size_t len, bool negative,
									   numeric_base_t base,
									   printf_size_t precision,
									   printf_size_t width,
									   printf_flags_t flags)
{
	printf_size_t unpadded_len = len;

	{
		if(!(flags & FLAGS_LEFT)) {
			if(width && (flags & FLAGS_ZEROPAD) &&
			   (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
				width--;
			}
			while((flags & FLAGS_ZEROPAD) && (len < width) &&
				  (len < PRINTF_INTEGER_BUFFER_SIZE)) {
				buf[len++] = '0';
			}
		}

		while((len < precision) && (len < PRINTF_INTEGER_BUFFER_SIZE)) {
			buf[len++] = '0';
		}

		if(base == BASE_OCTAL && (len > unpadded_len)) {
			flags &= ~FLAGS_HASH;
		}
	}

	if(flags & (FLAGS_HASH | FLAGS_POINTER)) {
		if(!(flags & FLAGS_PRECISION) && len &&
		   ((len == precision) || (len == width))) {
			if(unpadded_len < len) {
				len--;
			}
			if(len && (base == BASE_HEX || base == BASE_BINARY) &&
			   (unpadded_len < len)) {
				len--;
			}
		}
		if((base == BASE_HEX) && !(flags & FLAGS_UPPERCASE) &&
		   (len < PRINTF_INTEGER_BUFFER_SIZE)) {
			buf[len++] = 'x';
		} else if((base == BASE_HEX) && (flags & FLAGS_UPPERCASE) &&
				  (len < PRINTF_INTEGER_BUFFER_SIZE)) {
			buf[len++] = 'X';
		} else if((base == BASE_BINARY) && (len < PRINTF_INTEGER_BUFFER_SIZE)) {
			buf[len++] = 'b';
		}
		if(len < PRINTF_INTEGER_BUFFER_SIZE) {
			buf[len++] = '0';
		}
	}

	if(len < PRINTF_INTEGER_BUFFER_SIZE) {
		if(negative) {
			buf[len++] = '-';
		} else if(flags & FLAGS_PLUS) {
			buf[len++] = '+';
		} else if(flags & FLAGS_SPACE) {
			buf[len++] = ' ';
		}
	}

	out_rev_(output, buf, len, width, flags);
}

static void print_integer(output_gadget_t *output,
						  printf_unsigned_value_t value, bool negative,
						  numeric_base_t base, printf_size_t precision,
						  printf_size_t width, printf_flags_t flags)
{
	char buf[PRINTF_INTEGER_BUFFER_SIZE];
	printf_size_t len = 0U;

	if(!value) {
		if(!(flags & FLAGS_PRECISION)) {
			buf[len++] = '0';
			flags &= ~FLAGS_HASH;
		} else if(base == BASE_HEX) {
			flags &= ~FLAGS_HASH;
		}
	} else {
		do {
			const char digit = (char)(value % base);
			buf[len++] =
				(char)(digit < 10 ?
						   '0' + digit :
						   (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10);
			value /= base;
		} while(value && (len < PRINTF_INTEGER_BUFFER_SIZE));
	}

	print_integer_finalization(output, buf, len, negative, base, precision,
							   width, flags);
}

#if(PRINTF_SUPPORT_DECIMAL_SPECIFIERS || PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS)

struct double_components {
	int_fast64_t integral;
	int_fast64_t fractional;
	bool is_negative;
};

#define NUM_DECIMAL_DIGITS_IN_INT64_T 18
#define PRINTF_MAX_PRECOMPUTED_POWER_OF_10 NUM_DECIMAL_DIGITS_IN_INT64_T
static const double powers_of_10[NUM_DECIMAL_DIGITS_IN_INT64_T] = {
	1e00, 1e01, 1e02, 1e03, 1e04, 1e05, 1e06, 1e07, 1e08,
	1e09, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17
};

#define PRINTF_MAX_SUPPORTED_PRECISION NUM_DECIMAL_DIGITS_IN_INT64_T - 1

static struct double_components get_components(double number,
											   printf_size_t precision)
{
	struct double_components number_;
	number_.is_negative = get_sign_bit(number);
	double abs_number = (number_.is_negative) ? -number : number;
	number_.integral = (int_fast64_t)abs_number;
	double remainder =
		(abs_number - (double)number_.integral) * powers_of_10[precision];
	number_.fractional = (int_fast64_t)remainder;

	remainder -= (double)number_.fractional;

	if(remainder > 0.5) {
		++number_.fractional;
		if((double)number_.fractional >= powers_of_10[precision]) {
			number_.fractional = 0;
			++number_.integral;
		}
	} else if((remainder == 0.5) &&
			  ((number_.fractional == 0U) || (number_.fractional & 1U))) {
		++number_.fractional;
	}

	if(precision == 0U) {
		remainder = abs_number - (double)number_.integral;
		if((!(remainder < 0.5) || (remainder > 0.5)) &&
		   (number_.integral & 1)) {
			++number_.integral;
		}
	}
	return number_;
}

#if PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS
struct scaling_factor {
	double raw_factor;
	bool multiply;
};

static double apply_scaling(double num, struct scaling_factor normalization)
{
	return normalization.multiply ? num * normalization.raw_factor :
									num / normalization.raw_factor;
}

static double unapply_scaling(double normalized,
							  struct scaling_factor normalization)
{
	return normalization.multiply ? normalized / normalization.raw_factor :
									normalized * normalization.raw_factor;
}

static struct scaling_factor
update_normalization(struct scaling_factor sf,
					 double extra_multiplicative_factor)
{
	struct scaling_factor result;
	if(sf.multiply) {
		result.multiply = true;
		result.raw_factor = sf.raw_factor * extra_multiplicative_factor;
	} else {
		int factor_exp2 = get_exp2(get_bit_access(sf.raw_factor));
		int extra_factor_exp2 =
			get_exp2(get_bit_access(extra_multiplicative_factor));

		if(PRINTF_ABS(factor_exp2) > PRINTF_ABS(extra_factor_exp2)) {
			result.multiply = false;
			result.raw_factor = sf.raw_factor / extra_multiplicative_factor;
		} else {
			result.multiply = true;
			result.raw_factor = extra_multiplicative_factor / sf.raw_factor;
		}
	}
	return result;
}

static struct double_components get_normalized_components(
	bool negative, printf_size_t precision, double non_normalized,
	struct scaling_factor normalization, int floored_exp10)
{
	struct double_components components;
	components.is_negative = negative;
	double scaled = apply_scaling(non_normalized, normalization);

	bool close_to_representation_extremum =
		((-floored_exp10 + (int)precision) >= DBL_MAX_10_EXP - 1);
	if(close_to_representation_extremum) {
		return get_components(negative ? -scaled : scaled, precision);
	}
	components.integral = (int_fast64_t)scaled;
	double remainder =
		non_normalized -
		unapply_scaling((double)components.integral, normalization);
	double prec_power_of_10 = powers_of_10[precision];
	struct scaling_factor account_for_precision =
		update_normalization(normalization, prec_power_of_10);
	double scaled_remainder = apply_scaling(remainder, account_for_precision);
	double rounding_threshold = 0.5;

	components.fractional = (int_fast64_t)scaled_remainder;
	scaled_remainder -= (double)components.fractional;

	components.fractional += (scaled_remainder >= rounding_threshold);
	if(scaled_remainder == rounding_threshold) {
		components.fractional &= ~((int_fast64_t)0x1);
	}
	if((double)components.fractional >= prec_power_of_10) {
		components.fractional = 0;
		++components.integral;
	}
	return components;
}
#endif /* PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS */

static void print_broken_up_decimal(struct double_components number_,
									output_gadget_t *output,
									printf_size_t precision,
									printf_size_t width, printf_flags_t flags,
									char *buf, printf_size_t len)
{
	if(precision != 0U) {
		printf_size_t count = precision;

		if((flags & FLAGS_ADAPT_EXP) && !(flags & FLAGS_HASH) &&
		   (number_.fractional > 0)) {
			while(true) {
				int_fast64_t digit = number_.fractional % 10U;
				if(digit != 0) {
					break;
				}
				--count;
				number_.fractional /= 10U;
			}
		}

		if(number_.fractional > 0 || !(flags & FLAGS_ADAPT_EXP) ||
		   (flags & FLAGS_HASH)) {
			while(len < PRINTF_DECIMAL_BUFFER_SIZE) {
				--count;
				buf[len++] = (char)('0' + number_.fractional % 10U);
				if(!(number_.fractional /= 10U)) {
					break;
				}
			}
			while((len < PRINTF_DECIMAL_BUFFER_SIZE) && (count > 0U)) {
				buf[len++] = '0';
				--count;
			}
			if(len < PRINTF_DECIMAL_BUFFER_SIZE) {
				buf[len++] = '.';
			}
		}
	} else {
		if((flags & FLAGS_HASH) && (len < PRINTF_DECIMAL_BUFFER_SIZE)) {
			buf[len++] = '.';
		}
	}

	while(len < PRINTF_DECIMAL_BUFFER_SIZE) {
		buf[len++] = (char)('0' + (number_.integral % 10));
		if(!(number_.integral /= 10)) {
			break;
		}
	}

	if(!(flags & FLAGS_LEFT) && (flags & FLAGS_ZEROPAD)) {
		if(width &&
		   (number_.is_negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
			width--;
		}
		while((len < width) && (len < PRINTF_DECIMAL_BUFFER_SIZE)) {
			buf[len++] = '0';
		}
	}

	if(len < PRINTF_DECIMAL_BUFFER_SIZE) {
		if(number_.is_negative) {
			buf[len++] = '-';
		} else if(flags & FLAGS_PLUS) {
			buf[len++] = '+';
		} else if(flags & FLAGS_SPACE) {
			buf[len++] = ' ';
		}
	}

	out_rev_(output, buf, len, width, flags);
}

static void print_decimal_number(output_gadget_t *output, double number,
								 printf_size_t precision, printf_size_t width,
								 printf_flags_t flags, char *buf,
								 printf_size_t len)
{
	struct double_components value_ = get_components(number, precision);
	print_broken_up_decimal(value_, output, precision, width, flags, buf, len);
}

#if PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS

static int b_floor(double x)
{
	if(x >= 0) {
		return (int)x;
	}
	int n = (int)x;
	return (((double)n) == x) ? n : n - 1;
}

static double log10_of_positive(double positive_number)
{
	double_with_bit_access dwba = get_bit_access(positive_number);
	int exp2 = get_exp2(dwba);
	dwba.U =
		(dwba.U & (((double_uint_t)(1) << DOUBLE_STORED_MANTISSA_BITS) - 1U)) |
		((double_uint_t)DOUBLE_BASE_EXPONENT << DOUBLE_STORED_MANTISSA_BITS);
	double z = (dwba.F - 1.5);
	return (0.1760912590556812420 + z * 0.2895296546021678851
#if PRINTF_LOG10_TAYLOR_TERMS > 2
			- z * z * 0.0965098848673892950
#if PRINTF_LOG10_TAYLOR_TERMS > 3
			+ z * z * z * 0.0428932821632841311
#endif
#endif
			+ exp2 * 0.30102999566398119521);
}

static double pow10_of_int(int floored_exp10)
{
	if(floored_exp10 == DOUBLE_MAX_SUBNORMAL_EXPONENT_OF_10) {
		return DOUBLE_MAX_SUBNORMAL_POWER_OF_10;
	}
	double_with_bit_access dwba;
	int exp2 = b_floor(floored_exp10 * 3.321928094887362 + 0.5);
	const double z =
		floored_exp10 * 2.302585092994046 - exp2 * 0.6931471805599453;
	const double z2 = z * z;
	dwba.U = ((double_uint_t)(exp2) + DOUBLE_BASE_EXPONENT)
			 << DOUBLE_STORED_MANTISSA_BITS;
	dwba.F *= 1 + 2 * z / (2 - z + (z2 / (6 + (z2 / (10 + z2 / 14)))));
	return dwba.F;
}

static void print_exponential_number(output_gadget_t *output, double number,
									 printf_size_t precision,
									 printf_size_t width, printf_flags_t flags,
									 char *buf, printf_size_t len)
{
	const bool negative = get_sign_bit(number);
	double abs_number = negative ? -number : number;

	int floored_exp10;
	bool abs_exp10_covered_by_powers_table;
	struct scaling_factor normalization;

	if(abs_number == 0.0) {
		floored_exp10 = 0;
	} else {
		double exp10 = log10_of_positive(abs_number);
		floored_exp10 = b_floor(exp10);
		double p10 = pow10_of_int(floored_exp10);
		if(abs_number < p10) {
			floored_exp10--;
			p10 /= 10;
		}
		abs_exp10_covered_by_powers_table = PRINTF_ABS(floored_exp10) <
											PRINTF_MAX_PRECOMPUTED_POWER_OF_10;
		normalization.raw_factor = abs_exp10_covered_by_powers_table ?
									   powers_of_10[PRINTF_ABS(floored_exp10)] :
									   p10;
	}

	bool fall_back_to_decimal_only_mode = false;
	if(flags & FLAGS_ADAPT_EXP) {
		int required_significant_digits = (precision == 0) ? 1 : (int)precision;
		fall_back_to_decimal_only_mode =
			(floored_exp10 >= -4 &&
			 floored_exp10 < required_significant_digits);
		int precision_ = fall_back_to_decimal_only_mode ?
							 (int)precision - 1 - floored_exp10 :
							 (int)precision - 1;
		precision = (precision_ > 0 ? (unsigned)precision_ : 0U);
		flags |= FLAGS_PRECISION;
	}

	normalization.multiply =
		(floored_exp10 < 0 && abs_exp10_covered_by_powers_table);
	bool should_skip_normalization =
		(fall_back_to_decimal_only_mode || floored_exp10 == 0);
	struct double_components decimal_part_components =
		should_skip_normalization ?
			get_components(negative ? -abs_number : abs_number, precision) :
			get_normalized_components(negative, precision, abs_number,
									  normalization, floored_exp10);

	if(fall_back_to_decimal_only_mode) {
		if((flags & FLAGS_ADAPT_EXP) && floored_exp10 >= -1 &&
		   decimal_part_components.integral ==
			   powers_of_10[floored_exp10 + 1]) {
			floored_exp10++;
			precision--;
		}
	} else {
		if(decimal_part_components.integral >= 10) {
			floored_exp10++;
			decimal_part_components.integral = 1;
			decimal_part_components.fractional = 0;
		}
	}

	printf_size_t exp10_part_width = fall_back_to_decimal_only_mode	   ? 0U :
									 (PRINTF_ABS(floored_exp10) < 100) ? 4U :
																		 5U;

	printf_size_t decimal_part_width =
		((flags & FLAGS_LEFT) && exp10_part_width) ?
			0U :
			((width > exp10_part_width) ? width - exp10_part_width : 0U);

	const printf_size_t printed_exponential_start_pos = output->pos;
	print_broken_up_decimal(decimal_part_components, output, precision,
							decimal_part_width, flags, buf, len);

	if(!fall_back_to_decimal_only_mode) {
		putchar_via_gadget(output, (flags & FLAGS_UPPERCASE) ? 'E' : 'e');
		print_integer(output, ABS_FOR_PRINTING(floored_exp10),
					  floored_exp10 < 0, 10, 0, exp10_part_width - 1,
					  FLAGS_ZEROPAD | FLAGS_PLUS);
		if(flags & FLAGS_LEFT) {
			while(output->pos - printed_exponential_start_pos < width) {
				putchar_via_gadget(output, ' ');
			}
		}
	}
}
#endif

static void print_floating_point(output_gadget_t *output, double value,
								 printf_size_t precision, printf_size_t width,
								 printf_flags_t flags, bool prefer_exponential)
{
	char buf[PRINTF_DECIMAL_BUFFER_SIZE];
	printf_size_t len = 0U;

	if(value != value) {
		out_rev_(output, "nan", 3, width, flags);
		return;
	}
	if(value < -DBL_MAX) {
		out_rev_(output, "fni-", 4, width, flags);
		return;
	}
	if(value > DBL_MAX) {
		out_rev_(output, (flags & FLAGS_PLUS) ? "fni+" : "fni",
				 (flags & FLAGS_PLUS) ? 4U : 3U, width, flags);
		return;
	}

	if(!prefer_exponential && ((value > PRINTF_FLOAT_NOTATION_THRESHOLD) ||
							   (value < -PRINTF_FLOAT_NOTATION_THRESHOLD))) {
#if PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS
		print_exponential_number(output, value, precision, width, flags, buf,
								 len);
#endif
		return;
	}

	if(!(flags & FLAGS_PRECISION)) {
		precision = PRINTF_DEFAULT_FLOAT_PRECISION;
	}

	while((len < PRINTF_DECIMAL_BUFFER_SIZE) &&
		  (precision > PRINTF_MAX_SUPPORTED_PRECISION)) {
		buf[len++] = '0';
		precision--;
	}

#if PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS
	if(prefer_exponential)
		print_exponential_number(output, value, precision, width, flags, buf,
								 len);
	else
#endif
		print_decimal_number(output, value, precision, width, flags, buf, len);
}

#endif

static printf_flags_t parse_flags(const char **format)
{
	printf_flags_t flags = 0U;
	do {
		switch(**format) {
		case '0':
			flags |= FLAGS_ZEROPAD;
			(*format)++;
			break;
		case '-':
			flags |= FLAGS_LEFT;
			(*format)++;
			break;
		case '+':
			flags |= FLAGS_PLUS;
			(*format)++;
			break;
		case ' ':
			flags |= FLAGS_SPACE;
			(*format)++;
			break;
		case '#':
			flags |= FLAGS_HASH;
			(*format)++;
			break;
		default:
			return flags;
		}
	} while(true);
}

static inline void format_string_loop(output_gadget_t *output,
									  const char *format, va_list args)
{
#if PRINTF_CHECK_FOR_NUL_IN_FORMAT_SPECIFIER
#define ADVANCE_IN_FORMAT_STRING(cptr_) \
	do {                                \
		(cptr_)++;                      \
		if(!*(cptr_))                   \
			return;                     \
	} while(0)
#else
#define ADVANCE_IN_FORMAT_STRING(cptr_) (cptr_)++
#endif

	while(*format) {
		if(*format != '%') {
			putchar_via_gadget(output, *format);
			format++;
			continue;
		}
		ADVANCE_IN_FORMAT_STRING(format);

		printf_flags_t flags = parse_flags(&format);

		printf_size_t width = 0U;
		if(is_digit_(*format)) {
			width = (printf_size_t)atou_(&format);
		} else if(*format == '*') {
			const int w = va_arg(args, int);
			if(w < 0) {
				flags |= FLAGS_LEFT;
				width = (printf_size_t)-w;
			} else {
				width = (printf_size_t)w;
			}
			ADVANCE_IN_FORMAT_STRING(format);
		}

		printf_size_t precision = 0U;
		if(*format == '.') {
			flags |= FLAGS_PRECISION;
			ADVANCE_IN_FORMAT_STRING(format);
			if(is_digit_(*format)) {
				precision = (printf_size_t)atou_(&format);
			} else if(*format == '*') {
				const int precision_ = va_arg(args, int);
				precision = precision_ > 0 ? (printf_size_t)precision_ : 0U;
				ADVANCE_IN_FORMAT_STRING(format);
			}
		}

		switch(*format) {
#ifdef PRINTF_SUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS
		case 'I': {
			ADVANCE_IN_FORMAT_STRING(format);
			switch(*format) {
			case '8':
				flags |= FLAGS_INT8;
				ADVANCE_IN_FORMAT_STRING(format);
				break;
			case '1':
				ADVANCE_IN_FORMAT_STRING(format);
				if(*format == '6') {
					format++;
					flags |= FLAGS_INT16;
				}
				break;
			case '3':
				ADVANCE_IN_FORMAT_STRING(format);
				if(*format == '2') {
					ADVANCE_IN_FORMAT_STRING(format);
					flags |= FLAGS_INT32;
				}
				break;
			case '6':
				ADVANCE_IN_FORMAT_STRING(format);
				if(*format == '4') {
					ADVANCE_IN_FORMAT_STRING(format);
					flags |= FLAGS_INT64;
				}
				break;
			default:
				break;
			}
			break;
		}
#endif
		case 'l':
			flags |= FLAGS_LONG;
			ADVANCE_IN_FORMAT_STRING(format);
			if(*format == 'l') {
				flags |= FLAGS_LONG_LONG;
				ADVANCE_IN_FORMAT_STRING(format);
			}
			break;
		case 'h':
			flags |= FLAGS_SHORT;
			ADVANCE_IN_FORMAT_STRING(format);
			if(*format == 'h') {
				flags |= FLAGS_CHAR;
				ADVANCE_IN_FORMAT_STRING(format);
			}
			break;
		case 't':
			flags |= (sizeof(ptrdiff_t) == sizeof(long) ? FLAGS_LONG :
														  FLAGS_LONG_LONG);
			ADVANCE_IN_FORMAT_STRING(format);
			break;
		case 'j':
			flags |= (sizeof(intmax_t) == sizeof(long) ? FLAGS_LONG :
														 FLAGS_LONG_LONG);
			ADVANCE_IN_FORMAT_STRING(format);
			break;
		case 'z':
			flags |=
				(sizeof(size_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
			ADVANCE_IN_FORMAT_STRING(format);
			break;
		default:
			break;
		}

		switch(*format) {
		case 'd':
		case 'i':
		case 'u':
		case 'x':
		case 'X':
		case 'o':
		case 'b': {
			if(*format == 'd' || *format == 'i') {
				flags |= FLAGS_SIGNED;
			}

			numeric_base_t base;
			if(*format == 'x' || *format == 'X') {
				base = BASE_HEX;
			} else if(*format == 'o') {
				base = BASE_OCTAL;
			} else if(*format == 'b') {
				base = BASE_BINARY;
			} else {
				base = BASE_DECIMAL;
				flags &= ~FLAGS_HASH;
			}

			if(*format == 'X') {
				flags |= FLAGS_UPPERCASE;
			}

			format++;
			if(flags & FLAGS_PRECISION) {
				flags &= ~FLAGS_ZEROPAD;
			}

			if(flags & FLAGS_SIGNED) {
				if(flags & FLAGS_LONG_LONG) {
#if PRINTF_SUPPORT_LONG_LONG
					const long long value = va_arg(args, long long);
					print_integer(output, ABS_FOR_PRINTING(value), value < 0,
								  base, precision, width, flags);
#endif
				} else if(flags & FLAGS_LONG) {
					const long value = va_arg(args, long);
					print_integer(output, ABS_FOR_PRINTING(value), value < 0,
								  base, precision, width, flags);
				} else {
					const int value =
						(flags & FLAGS_CHAR)  ? (signed char)va_arg(args, int) :
						(flags & FLAGS_SHORT) ? (short int)va_arg(args, int) :
												va_arg(args, int);
					print_integer(output, ABS_FOR_PRINTING(value), value < 0,
								  base, precision, width, flags);
				}
			} else {
				flags &= ~(FLAGS_PLUS | FLAGS_SPACE);

				if(flags & FLAGS_LONG_LONG) {
#if PRINTF_SUPPORT_LONG_LONG
					print_integer(output,
								  (printf_unsigned_value_t)va_arg(
									  args, unsigned long long),
								  false, base, precision, width, flags);
#endif
				} else if(flags & FLAGS_LONG) {
					print_integer(
						output,
						(printf_unsigned_value_t)va_arg(args, unsigned long),
						false, base, precision, width, flags);
				} else {
					const unsigned int value =
						(flags & FLAGS_CHAR) ?
							(unsigned char)va_arg(args, unsigned int) :
						(flags & FLAGS_SHORT) ?
							(unsigned short int)va_arg(args, unsigned int) :
							va_arg(args, unsigned int);
					print_integer(output, (printf_unsigned_value_t)value, false,
								  base, precision, width, flags);
				}
			}
			break;
		}
#if PRINTF_SUPPORT_DECIMAL_SPECIFIERS
		case 'f':
		case 'F':
			if(*format == 'F')
				flags |= FLAGS_UPPERCASE;
			print_floating_point(output, va_arg(args, double), precision, width,
								 flags, PRINTF_PREFER_DECIMAL);
			format++;
			break;
#endif
#if PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS
		case 'e':
		case 'E':
		case 'g':
		case 'G':
			if((*format == 'g') || (*format == 'G'))
				flags |= FLAGS_ADAPT_EXP;
			if((*format == 'E') || (*format == 'G'))
				flags |= FLAGS_UPPERCASE;
			print_floating_point(output, va_arg(args, double), precision, width,
								 flags, PRINTF_PREFER_EXPONENTIAL);
			format++;
			break;
#endif
		case 'c': {
			printf_size_t l = 1U;
			if(!(flags & FLAGS_LEFT)) {
				while(l++ < width) {
					putchar_via_gadget(output, ' ');
				}
			}
			putchar_via_gadget(output, (char)va_arg(args, int));
			if(flags & FLAGS_LEFT) {
				while(l++ < width) {
					putchar_via_gadget(output, ' ');
				}
			}
			format++;
			break;
		}

		case 's': {
			const char *p = va_arg(args, char *);
			if(p == NULL) {
				out_rev_(output, ")llun(", 6, width, flags);
			} else {
				printf_size_t l = strnlen_s_(
					p, precision ? precision : PRINTF_MAX_POSSIBLE_BUFFER_SIZE);
				// pre padding
				if(flags & FLAGS_PRECISION) {
					l = (l < precision ? l : precision);
				}
				if(!(flags & FLAGS_LEFT)) {
					while(l++ < width) {
						putchar_via_gadget(output, ' ');
					}
				}
				while((*p != 0) && (!(flags & FLAGS_PRECISION) || precision)) {
					putchar_via_gadget(output, *(p++));
					--precision;
				}
				if(flags & FLAGS_LEFT) {
					while(l++ < width) {
						putchar_via_gadget(output, ' ');
					}
				}
			}
			format++;
			break;
		}

		case 'p': {
			width = sizeof(void *) * 2U + 2;
			flags |= FLAGS_ZEROPAD | FLAGS_POINTER;
			uintptr_t value = (uintptr_t)va_arg(args, void *);
			(value == (uintptr_t)NULL) ?
				out_rev_(output, ")lun(", 5, width, flags) :
				print_integer(output, (printf_unsigned_value_t)value, false,
							  BASE_HEX, precision, width, flags);
			format++;
			break;
		}

		case '%':
			putchar_via_gadget(output, '%');
			format++;
			break;

#if PRINTF_SUPPORT_WRITEBACK_SPECIFIER
		case 'n': {
			if(flags & FLAGS_CHAR)
				*(va_arg(args, char *)) = (char)output->pos;
			else if(flags & FLAGS_SHORT)
				*(va_arg(args, short *)) = (short)output->pos;
			else if(flags & FLAGS_LONG)
				*(va_arg(args, long *)) = (long)output->pos;
#if PRINTF_SUPPORT_LONG_LONG
			else if(flags & FLAGS_LONG_LONG)
				*(va_arg(args, long long *)) = (long long int)output->pos;
#endif
			else
				*(va_arg(args, int *)) = (int)output->pos;
			format++;
			break;
		}
#endif

		default:
			putchar_via_gadget(output, *format);
			format++;
			break;
		}
	}
}

static int vsnprintf_impl(output_gadget_t *output, const char *format,
						  va_list args)
{
	format_string_loop(output, format, args);
	append_termination_with_gadget(output);
	return (int)output->pos;
}

int vprintf(const char *format, va_list arg)
{
	output_gadget_t gadget = extern_putchar_gadget();
	return vsnprintf_impl(&gadget, format, arg);
}

int vsnprintf(char *s, size_t n, const char *format, va_list arg)
{
	output_gadget_t gadget = buffer_gadget(s, n);
	return vsnprintf_impl(&gadget, format, arg);
}

int vsprintf(char *s, const char *format, va_list arg)
{
	return vsnprintf(s, PRINTF_MAX_POSSIBLE_BUFFER_SIZE, format, arg);
}

int vfctprintf(void (*out)(char c, void *extra_arg), void *extra_arg,
			   const char *format, va_list arg)
{
	output_gadget_t gadget = function_gadget(out, extra_arg);
	return vsnprintf_impl(&gadget, format, arg);
}

int printf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	const int ret = vprintf(format, args);
	va_end(args);
	return ret;
}

int sprintf(char *s, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	const int ret = vsprintf(s, format, args);
	va_end(args);
	return ret;
}

int snprintf(char *s, size_t n, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	const int ret = vsnprintf(s, n, format, args);
	va_end(args);
	return ret;
}

int fctprintf(void (*out)(char c, void *extra_arg), void *extra_arg,
			  const char *format, ...)
{
	va_list args;
	va_start(args, format);
	const int ret = vfctprintf(out, extra_arg, format, args);
	va_end(args);
	return ret;
}
