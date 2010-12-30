/*
 * Gtranscoder
 *
 * Copyright (c) 2010 Alexandre Quessy <alexandre@quessy.net>
 *
 * Gtranscoder is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gtranscoder is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the gnu general public license
 * along with Gtranscoder.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <gconf/gconf.h> // GConf2 will be eventually replaced by GSettings
#include "application.h"
#include "gtranscoder-error.h"
#include "config.h"

static const gchar *APP_ONE_LINER = N_("Movie format converter");

static gboolean on_delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
    return FALSE;
}

static void on_window_destroyed(GtkWidget *widget, gpointer data)
{
    g_debug("`%s' called", __FUNCTION__);
    gtk_main_quit();
}

static void on_about_menu_item_chosen(GtkWidget *item, gpointer data)
{
    g_debug("`%s' called", __FUNCTION__);
}

static void on_help_about_chosen(GtkWidget *item, gpointer data)
{
    g_debug("`%s' called", __FUNCTION__);
}

static void on_help_index_chosen(GtkWidget *item, gpointer data)
{
    g_debug("`%s' called", __FUNCTION__);
}

static void on_quit_chosen(GtkWidget *item, gpointer data)
{
    g_debug("`%s' called", __FUNCTION__);
    gtk_main_quit();
}

static void on_error_dialog_response(GtkWidget *widget, gpointer data)
{
    g_debug("`%s' called", __FUNCTION__);
    gtk_main_quit();
}

static void on_containers_changed(GtkWidget *widget, gpointer data)
{
    g_debug("`%s' called", __FUNCTION__);
    gint key = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
    gchar *value = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));
    g_debug("Chosen container is %d (%s)", key, value);
    g_free(value);
    // will use that somewhere else:
    // gint num_items = gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(widget)), NULL);
    // if (num_items > 0)
    // {
    //     gtk_combo_box_remove_text(GTK_COMBO_BOX(widget), 0);
    //     g_debug("Removed item 0. %d are left.", num_items);
    // }
}

static void on_apply_clicked(GtkToolButton *widget, gpointer data)
{
    g_debug("`%s' called", __FUNCTION__);
    GtranscoderApp *app = (GtranscoderApp *) data;
    //app->file_chooser;
    GtkWidget *dialog = gtranscoder_show_error_dialog("The transcoding is not yet implemented.", NULL);
    // FIXME: why is this not working?
    g_signal_connect(dialog,
        "response",
        G_CALLBACK (on_error_dialog_response),
        NULL);
}

static void init_menubar(GtkWidget *window, GtkWidget *vbox, GtranscoderApp *app)
{
    /* setup shortcuts */
    GtkAccelGroup *accel_group = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);
    GtkWidget *menubar = gtk_menu_bar_new();

    /* Populate the file menu */
    GtkWidget *file_menu_widget = gtk_menu_item_new_with_mnemonic("_File");
    GtkWidget *file_menu = gtk_menu_new();
    GtkWidget *quit_menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_widget), GTK_WIDGET(file_menu));
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_menu_widget);
    g_signal_connect(quit_menu_item, "activate", G_CALLBACK(on_quit_chosen), NULL);
  
    /* make a help menu */
    GtkWidget *help_menu_widget = gtk_menu_item_new_with_mnemonic("_Help");
    GtkWidget *help_menu = gtk_menu_new();
    GtkWidget *help_help = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, accel_group);
    GtkWidget *help_about = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, accel_group);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_menu_widget), GTK_WIDGET(help_menu));
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_about);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help_menu_widget);
    g_signal_connect(help_help, "activate", G_CALLBACK(on_help_index_chosen), NULL);
    g_signal_connect(help_about, "activate", G_CALLBACK(on_help_about_chosen), NULL);

    /* oddly, it seems we need to explicitely show the menu bar so that it's visible. */
    gtk_widget_show_all(GTK_WIDGET(menubar));

    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(menubar), FALSE /* expand */, TRUE /* fill */, 0);
}

static void init_toolbar(GtkWidget *window, GtkWidget *vbox, GtranscoderApp *app)
{
    GtkWidget *toolbar = gtk_toolbar_new();

    /* the toolbar buttons */
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);
    GtkToolItem *apply_tool = gtk_tool_button_new_from_stock(GTK_STOCK_APPLY);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(apply_tool), N_("Apply"));
    gtk_widget_set_tooltip_text(GTK_WIDGET(apply_tool), N_("Proceed with the movie transcoding"));
    g_signal_connect(GTK_TOOL_BUTTON(apply_tool), "clicked", G_CALLBACK(on_apply_clicked), app);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), apply_tool, -1);
    gtk_widget_show_all(GTK_WIDGET(toolbar));

    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(toolbar), FALSE /* expand */, TRUE /* fill */, 0);
}

static void init_contents(GtkWidget *window, GtkWidget *vbox, GtranscoderApp *app)
{
    /* file chooser */
    GtkWidget *file_chooser = gtk_file_chooser_button_new(N_("Input file chooser"), GTK_FILE_CHOOSER_ACTION_OPEN);
    //g_signal_connect(file_chooser, "activate", G_CALLBACK(fileentry_activated), NULL);
    app->file_chooser = file_chooser;

    // Create the widgets to pack
    const gint num_rows = 2; // It's very important that this number matches in the next three lines:
    gchar *labels[2];
    GtkWidget *widgets[2];
    // row 0:
    labels[0] = N_("Input movie file:");
    widgets[0] = GTK_WIDGET(g_object_new(GTK_TYPE_ALIGNMENT,
                                          "xalign", 0.0,
                                          "child", file_chooser,
                                          NULL));
    labels[1] = N_("Container format:");
    GtkWidget *containers_combo_box = gtk_combo_box_new_text();
    gtk_combo_box_append_text(GTK_COMBO_BOX(containers_combo_box), "OGG");
    gtk_combo_box_append_text(GTK_COMBO_BOX(containers_combo_box), "Quicktime");
    gtk_combo_box_append_text(GTK_COMBO_BOX(containers_combo_box), "AVI");
    g_signal_connect(GTK_COMBO_BOX(containers_combo_box), "changed", G_CALLBACK(on_containers_changed), NULL);

    widgets[1] = GTK_WIDGET(g_object_new(GTK_TYPE_ALIGNMENT,
                                          "xalign", 0.0,
                                          "child", containers_combo_box,
                                          NULL));

    /* create a two-column table for all of the widgets */
    GtkTable *table = g_object_new(GTK_TYPE_TABLE,
                       "n-rows", num_rows,
                       "n-columns", 2,
                       "column-spacing", 6,
                       "row-spacing", 6,
                       NULL);

    int i;
    for (i = 0; i < num_rows; i++)
    {
        gtk_table_attach(table, g_object_new(GTK_TYPE_LABEL,
                                          "label", labels[i],
                                          "use-underline", TRUE,
                                          "mnemonic-widget", widgets[i],
                                          "xalign", 1.0,
                                          "justify", GTK_JUSTIFY_RIGHT,
                                          NULL),
                      /* col: */ 0, 1, /* row: */ i, i + 1, 
                      GTK_EXPAND | GTK_FILL, 0, 0, 0);

        gtk_table_attach(table, widgets[i],
                      /* col: */ 1, 2, /* row: */ i, i + 1, 
                      GTK_EXPAND | GTK_FILL, 0, 0, 0);
    }
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(table), TRUE /* expand */, TRUE /* fill */, 0);
}

static void init_statusbar(GtkWidget *window, GtkWidget *vbox, GtranscoderApp *app)
{
    GtkWidget *statusbar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE /* expand */, TRUE /* fill */, 0);
    gtk_widget_show_all(GTK_WIDGET(statusbar));
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "Only for when creating the status bar"), N_("Ready"));
    app->statusbar = statusbar;
}

void run_main_window(GtranscoderOptions *options)
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Gtranscoder");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    GtranscoderApp *app = g_new0(GtranscoderApp, 1);
    g_signal_connect(window, "delete_event", G_CALLBACK(on_delete_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroyed), NULL);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    /* create menu/tool/status bars */
    init_menubar(window, vbox, app);
    init_toolbar(window, vbox, app);
    init_contents(window, vbox, app);
    init_statusbar(window, vbox, app);
  
    /* show it all */
    gtk_widget_show_all(GTK_WIDGET(window));
}

