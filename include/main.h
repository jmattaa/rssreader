#ifndef RSSREADER_MAIN_H
#define RSSREADER_MAIN_H

#define pfail(err, ...)                                                        \
    do {                                                                       \
        printf(__VA_ARGS__);                                                   \
        return (err);                                                          \
    } while (0)

extern int win_w, win_h;

#endif
