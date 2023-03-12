#ifndef CTYPE_H_
#define CTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _CTYPE_A 0x00000100L /* Alpha */
#define _CTYPE_C 0x00000200L /* Control */
#define _CTYPE_D 0x00000400L /* Digit */
#define _CTYPE_G 0x00000800L /* Graph */
#define _CTYPE_L 0x00001000L /* Lower */
#define _CTYPE_P 0x00002000L /* Punct */
#define _CTYPE_S 0x00004000L /* Space */
#define _CTYPE_U 0x00008000L /* Upper */
#define _CTYPE_X 0x00010000L /* X digit */
#define _CTYPE_B 0x00020000L /* Blank */
#define _CTYPE_R 0x00040000L /* Print */
#define _CTYPE_I 0x00080000L /* Ideogram */
#define _CTYPE_T 0x00100000L /* Special */
#define _CTYPE_Q 0x00200000L /* Phonogram */
#define _CTYPE_SW0 0x20000000L /* 0 width character */
#define _CTYPE_SW1 0x40000000L /* 1 width character */
#define _CTYPE_SW2 0x80000000L /* 2 width character */
#define _CTYPE_SW3 0xc0000000L /* 3 width character */
#define _CTYPE_SWM 0xe0000000L /* Mask for screen width data */
#define _CTYPE_SWS 30 /* Bits to shift to get width */

int isalnum(int ch);
int isalpha(int ch);
int isascii(int ch);
int isblank(int ch);
int iscntrl(int ch);
int isdigit(int ch);
int isgraph(int ch);
int islower(int ch);
int isprint(int ch);
int ispunct(int ch);
int isspace(int ch);
int isupper(int ch);
int isxdigit(int ch);
int tolower(int ch);
int toupper(int ch);
int toascii(int ch);

#ifdef __cplusplus
}
#endif

#endif /* CTYPE_H_ */
