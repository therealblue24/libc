#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *strdup(const char *str)
{
    char *copy = NULL;

    if(str) {
        size_t len = strlen(str) + 1;

        if((copy = malloc(len)) == NULL) {
            return (NULL);
        }

        memcpy(copy, str, len);
    }

    return (copy);
}
