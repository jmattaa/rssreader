#include "net.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

static size_t write_callback(void *contents, size_t size, size_t nmemb,
                             void *userp)
{
    size_t realsize = size * nmemb;
    net_mem *mem = (net_mem *)userp;

    char *newptr = realloc(mem->ptr, mem->len + realsize + 1);
    if (!newptr)
        return 0;

    mem->ptr = newptr;

    memcpy(mem->ptr + mem->len, contents, realsize);
    mem->len += realsize;
    mem->ptr[mem->len] = '\0';

    return realsize;
}

CURLcode net_recv(CURL *curl, const char *url, net_mem *mem)
{
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, mem);

    return curl_easy_perform(curl);
}

net_mem *net_mem_alloc(void)
{
    net_mem *mem = malloc(sizeof(net_mem));
    mem->ptr = NULL;
    mem->len = 0;
    return mem;
}

void net_mem_free(net_mem *mem)
{
    if (!mem)
        return;
    free(mem->ptr);
    free(mem);
}
