#include "gio/gio.h"
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
    rchannelset *channels = rss_recvlinkf(curl, "./rsslinks");
    GtkApplication *app = gtk_application_new("org.jmattaa.rssreader",
                                              G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(ui_activate), channels);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    rss_free_set(channels);
    curl_easy_cleanup(curl);
    return status;
}
