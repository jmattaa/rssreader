#ifndef RSSREADER_NET_H
#define RSSREADER_NET_H

#include "curl/curl.h"
#include <stddef.h>

typedef struct 
{
    char *ptr;
    size_t len;
} net_mem;

CURLcode net_recv(CURL *curl, const char *url, net_mem *mem);

net_mem *net_mem_alloc();
void net_mem_free(net_mem *mem);

#endif
