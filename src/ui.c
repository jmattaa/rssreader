#include "ui.h"
#include "glibconfig.h"
#include "gtk/gtkcssprovider.h"
#include "rss.h"
#include <gtk/gtk.h>

static GtkWidget *win;

static GtkWidget *ui_rsschannellist_new(rchannel **channels);
static GtkWidget *ui_rsschannelitems_new(rchannel *ch);
static GtkWidget *ui_rssitem_new(ritem *item, int is_channel);
static void ui_on_itemclicked(void *_, void *user_data);
static void ui_on_channelclicked(void *_, void *user_data);
static void ui_setup_css(void);

void ui_activate(GtkApplication *app, void *user_data)
{
    // null terminated array with channels
    rchannel **channels = (rchannel **)user_data;

    ui_setup_css();

    win = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(win), "RSS Reader");
    gtk_window_set_default_size(GTK_WINDOW(win), 800, 600);

    GtkWidget *rchannel_widget = ui_rsschannellist_new(channels);
    gtk_window_set_child(GTK_WINDOW(win), rchannel_widget);

    gtk_window_present(GTK_WINDOW(win));
}

static GtkWidget *ui_rsschannellist_new(rchannel **channels)
{
    GtkWidget *scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    GtkWidget *list = gtk_list_box_new();

    for (size_t i = 0; channels[i]; i++)
    {
        GtkWidget *item_widget = ui_rssitem_new((ritem *)channels[i], true);
        gtk_list_box_append(GTK_LIST_BOX(list), item_widget);
    }

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), list);
    return scrolled;
}

static GtkWidget *ui_rsschannelitems_new(rchannel *ch)
{
    GtkWidget *scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    GtkWidget *list = gtk_list_box_new();

    if (!ch || !ch->items)
    {
        GtkWidget *label = gtk_label_new("No RSS items available");
        gtk_list_box_append(GTK_LIST_BOX(list), label);
        gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), list);
        return scrolled;
    }

    for (size_t i = 0; i < ch->nitems; i++)
    {
        GtkWidget *item_widget = ui_rssitem_new(ch->items[i], false);
        gtk_list_box_append(GTK_LIST_BOX(list), item_widget);
    }

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), list);
    return scrolled;
}

// in this case a rssitem can be a channel or a item of a channel
static GtkWidget *ui_rssitem_new(ritem *item, int is_channel)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

    GtkWidget *btn = gtk_button_new_with_label(item->title);
    gtk_widget_set_name(btn, "btn");

    GtkWidget *back_btn = gtk_button_new_with_label("← Back");
    gtk_widget_set_name(back_btn, "btn");

    // Get the button's label to set text properties
    GtkWidget *label = gtk_button_get_child(GTK_BUTTON(btn));
    if (GTK_IS_LABEL(label))
    {
        gtk_label_set_xalign(GTK_LABEL(label), 0.0);
        gtk_label_set_wrap(GTK_LABEL(label), TRUE);
        gtk_label_set_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD);
    }

    g_signal_connect(
        G_OBJECT(btn), "clicked",
        G_CALLBACK(is_channel ? ui_on_channelclicked : ui_on_itemclicked),
        item);

    gtk_box_append(GTK_BOX(box), btn);

    return box;
}

static void ui_on_itemclicked(void *_, void *user_data)
{
    ritem *item = (ritem *)user_data;
    if (item->link)
        gtk_uri_launcher_launch(gtk_uri_launcher_new(item->link), NULL, NULL,
                                NULL, NULL);
}

static void ui_on_channelclicked(void *_, void *user_data)
{
    rchannel *ch = (rchannel *)user_data;

    // add a back btn
    GtkWidget *items = ui_rsschannelitems_new(ch);
    gtk_window_set_child(GTK_WINDOW(win), items);
}

static void ui_setup_css(void)
{
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_string(
        css,
        "window { background-color: #1d2021; } "
        "#btn { font-size: 16px; font-weight: 600; color: #ebdbb2; "
        "padding: 8px; border: none; background: transparent; "
        "box-shadow: none; outline: none; } "
        "#comments-btn { font-size: 12px; padding: 4px 8px; margin-top: 4px; "
        "background-color: #3c3836; color: #ebdbb2; border: 1px solid #665c54; "
        "} "
        "#comments-btn:hover { background-color: #665c54; } "
        "list row { background: transparent; border: none; } "
        "list row:selected { background: transparent; border: none; } "
        "list row:hover { background: #3c3836; } "
        "list { background-color: #1d2021; } "
        "label { color: #ebdbb2; } "
        ".caption { color: #928374; }");

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(), GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_USER);
}
