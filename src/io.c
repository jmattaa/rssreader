#include "io.h"
#include <stdio.h>
#include <stdlib.h>

char *io_read(const char *fp)
{
    FILE *f = fopen(fp, "r");
    if (!f)
        return NULL;

    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(len + 1);

    if (!buf)
    {
        fclose(f);
        free(buf);
        printf("unable to allocate memory for file %s\n", fp);
        return NULL;
    }

    fread(buf, len, 1, f);
    if (ferror(f))
    {
        free(buf);
        fclose(f);
        printf("unable to read file %s\n", fp);
        return NULL;
    }
    fclose(f);

    buf[len] = '\0';
    return buf;
}
