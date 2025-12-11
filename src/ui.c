#include "ui.h"
#include "rss.h"
#include <gtk/gtk.h>

static GtkWidget *ui_rsschannel_new(rchannel *ch);
static GtkWidget *ui_rssitem_new(ritem *item);

void ui_activate(GtkApplication *app, gpointer user_data)
{
    rchannel *ch = (rchannel *)user_data;

    GtkWidget *win = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(win), "RSS Reader");
    gtk_window_set_default_size(GTK_WINDOW(win), 800, 600);

    GtkWidget *rchannel_widget = ui_rsschannel_new(ch);
    gtk_window_set_child(GTK_WINDOW(win), rchannel_widget);

    gtk_window_present(GTK_WINDOW(win));
}

static GtkWidget *ui_rsschannel_new(rchannel *ch)
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
        GtkWidget *item_widget = ui_rssitem_new(ch->items[i]);
        gtk_list_box_append(GTK_LIST_BOX(list), item_widget);
    }

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), list);
    return scrolled;
}

static GtkWidget *ui_rssitem_new(ritem *item)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_start(box, 12);
    gtk_widget_set_margin_end(box, 12);
    gtk_widget_set_margin_top(box, 8);
    gtk_widget_set_margin_bottom(box, 8);

    GtkWidget *title = gtk_label_new(item->title);
    gtk_label_set_xalign(GTK_LABEL(title), 0.0);
    gtk_label_set_wrap(GTK_LABEL(title), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(title), PANGO_WRAP_WORD);
    gtk_widget_set_name(title, "rss-title");

    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_string(
        css, "list { background-color: #f8f9fa; }"
             "#rss-title { font-size: 16px; font-weight: 600; color: #2c3e50; "
             "margin-bottom: 4px; }"
             "list row { border-bottom: 1px solid #e9ecef; padding: 0px; }"
             "list row:hover:not(:selected) { background-color: #e9ecef; }");
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(), GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_box_append(GTK_BOX(box), title);

    return box;
}
