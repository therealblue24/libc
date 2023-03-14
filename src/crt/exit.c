#include <stdlib.h>

extern void _exit(int);

static void (*atexit_func[32])(void);
static void (*atquickexit_func[32])(void);
static int atexit_cnt;
static int atquickexit_cnt;

[[gnu::weak]] int atexit(void (*func)(void))
{
	if(atexit_cnt >= 32)
		return -1;
	atexit_func[atexit_cnt++] = func;
	return 0;
}

[[gnu::weak]] int at_quick_exit(void (*func)(void))
{
	if(atquickexit_cnt >= 32)
		return -1;
	atquickexit_func[atquickexit_cnt++] = func;
	return 0;
}

[[gnu::weak]] void __tr24_exit()
{
	for(int i = 0; i < atexit_cnt; i++) {
		atexit_func[i]();
	}
}

[[gnu::weak]] void __tr24_quick_exit()
{
	for(int i = 0; i < atquickexit_cnt; i++) {
		atquickexit_func[i]();
	}
}

[[gnu::weak]] void exit(int status)
{
	__tr24_exit();
	_exit(status);
}

[[gnu::weak]] void quick_exit(int status)
{
	__tr24_quick_exit();
	_exit(status);
}
