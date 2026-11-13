#ifndef RSS_READER_NET_H
#define RSS_READER_NET_H

#include <curl/curl.h>

typedef struct 
{
    char *ptr;
    size_t len;
} net_mem;

CURLcode net_recv(CURL *curl, const char *uri, net_mem *mem);

net_mem *net_mem_alloc();
void net_mem_free(net_mem *mem);

#endif
