#include "gio/gio.h"
#include "gtk/gtk.h"
#include "net.h"
#include "rss.h"
#include "ui.h"
#include <curl/curl.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        fprintf(stderr, "curl_easy_init failed\n");
        return 1;
    }

    net_mem *mem = mem_alloc();
    CURLcode res = net_recv(curl, "https://news.ycombinator.com/rss", mem);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "net_recv failed: %s\n", curl_easy_strerror(res));
        return 1;
    }

    rchannel *ch = rss_parse(mem->ptr, mem->len);
    if (!ch)
    {
        fprintf(stderr, "rss_parse failed\n");
        return 1;
    }

    GtkApplication *app =
        gtk_application_new("org.jmattaa.rssreader", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(ui_activate), ch);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    rss_free(ch);
    mem_free(mem);
    curl_easy_cleanup(curl);
    return status;
}
