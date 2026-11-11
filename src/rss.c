#include "rss.h"
#include <raygui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "main.h"
#include "raylib.h"

void rchan_draw(rchan *c, int i) {
    int starth = RCHAN_UI_H * i;
    if (GuiButton((Rectangle){0, starth, win_w, RCHAN_UI_H},
                  c->title)) {
        printf("%s\n", c->link);
    }
}

rchan *rchan_init(char *l, char *t) {
    rchan *ch = malloc(sizeof(rchan));
    if (!ch)
        pfail(NULL, "couldn't allocate memory for channel");

    ch->link = l;
    ch->title = t;

    return ch;
}

rchan **rss_parsef(const char *fp, int *n) {
    rchan **chs = NULL;

    char *rsslinks = io_readf(fp);
    char *l = strtok(rsslinks, "\n");

    *n = 0;
    while (l != NULL) {
        chs = realloc(chs, ((*n) + 1) * sizeof(*chs));
        if (!chs)
            pfail(NULL, "couldn't allocate memory while parsing \"%s\"", fp);

        chs[*n] = rchan_init(strdup(l), strdup(l));

        (*n)++;
        l = strtok(NULL, "\n");
    }

    free(rsslinks);

    return chs;
}

void rchan_free(rchan *ch) {
    if (ch->link)
        free(ch->link);
    if (ch->title)
        free(ch->title);
    free(ch);
}

void rchanarr_free(rchan **chs, int n) {
    for (int i = 0; i < n; i++)
        if (chs[i])
            rchan_free(chs[i]);

    free(chs);
}
