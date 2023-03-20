#include <string.h>

int strcmp(const char *s1, const char *s2)
{
    int r = -1;

    if(s1 == s2) {
        return 0;
    }

    if(s1 != NULL && s2 != NULL) {
        for(; *s1 == *s2; ++s1, ++s2) {
            if(*s1 == 0) {
                r = 0;
                break;
            }
        }

        if(r != 0) {
            r = *(const char *)s1 - *(const char *)s2;
        }
    }

    return r;
}
