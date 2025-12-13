#include "gio/gio.h"
#include "glibconfig.h"
#include "gtk/gtk.h"
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

    // now everything is pulled down from the network on startup
    // TODO: maybe store the links and download when clicked
    rchannel *channels[] = {
        rss_recvandparse(curl, "https://news.ycombinator.com/rss"),
        rss_recvandparse(
            curl, "https://news.un.org/feed/subscribe/en/news/all/rss.xml"),
        NULL};

    GtkApplication *app = gtk_application_new("org.jmattaa.rssreader",
                                              G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(ui_activate), channels);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    for (size_t i = 0; channels[i]; i++)
        rss_free(channels[i]);
    curl_easy_cleanup(curl);
    return status;
}
