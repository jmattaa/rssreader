#include "io.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

char *io_readf(const char *fp) {
    FILE *f = fopen(fp, "r");
    if (!f)
        pfail(NULL, "couldn't open \"%s\": %s", fp, strerror(errno));

    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(len + 1);

    if (!buf) {
        fclose(f);
        free(buf);
        pfail(NULL, "couldn't allocate memory for file: %s", fp);
    }

    fread(buf, len, 1, f);
    if (ferror(f)) {
        free(buf);
        fclose(f);
        pfail(NULL, "unable to read file %s\n", fp);
    }
    fclose(f);

    buf[len] = '\0';
    return buf;
}
