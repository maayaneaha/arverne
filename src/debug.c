#include "debug.h"

int init_debug()
{
    FILE *ptr = fopen("debug_file", "w");
    if (ptr == NULL)
        return 0;
    fprintf(ptr, "\n");
    fclose(ptr);
    return 1;
}

int debug_write(const char *s)
{
    FILE *ptr = fopen("debug_file", "a");
    if (ptr == NULL)
        return 0;
    int v = fprintf(ptr,"%s", s);
    fclose(ptr);
    return v;
}