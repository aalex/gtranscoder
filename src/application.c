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

// FIXME
#include <gnome.h>
#include <gtk/gtk.h>
//TODO: use gconf #include <gconf/gconf.h>
// FIXME
#include <libgnomeui/libgnomeui.h>
#include "application.h"
#include "gtranscoder-error.h"
#include "config.h"

// TODO: deprecate this (we don't need gobject set data)
static const gchar *APP_DATA_KEY = "app-data";
static const gchar *APP_ONE_LINER = N_("Movie format converter");

static GtranscoderApp *get_app(GtkWidget *window)
{
    return (GtranscoderApp *) g_object_get_data(G_OBJECT(window), APP_DATA_KEY);
}

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

static void on_error_dialog_response(GtkWidget *item, gpointer data)
{
    g_debug("`%s' called", __FUNCTION__);
    gtk_main_quit();
}

static void on_apply_clicked(GtkToolButton *toolbutton, gpointer data)
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

/* handler for GnomeFileEntry::activate */
static void fileentry_activated(GnomeFileEntry *file_widget, gpointer data)
{
    g_debug("`%s' called", __FUNCTION__);
    gchar *path;
    path = gnome_file_entry_get_full_path(file_widget, FALSE);
    g_debug("File chooser activated; new File: %s", path);
    g_free(path);
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
    GtkWidget *help_about = gtk_image_menu_item_new_from_stock(GNOME_STOCK_ABOUT, accel_group);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_menu_widget), GTK_WIDGET(help_menu));
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_about);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help_menu_widget);
    g_signal_connect(help_help, "activate", G_CALLBACK(on_help_index_chosen), NULL);
    g_signal_connect(help_about, "activate", G_CALLBACK(on_help_about_chosen), NULL);

    /* oddly, it seems we need to explicitely show the menu bar so that it's visible. */
    gtk_widget_show_all(GTK_WIDGET(menubar));

    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(menubar), FALSE, FALSE, 0);
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

    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(toolbar), FALSE, FALSE, 0);
}

static void init_contents(GtkWidget *window, GtkWidget *vbox, GtranscoderApp *app)
{
    /* file chooser */
    GtkWidget *file_chooser = g_object_new(GNOME_TYPE_FILE_ENTRY, 
        "history-id", "fileentry",
         NULL);
    g_signal_connect(file_chooser, "activate", G_CALLBACK(fileentry_activated), NULL);
    app->file_chooser = file_chooser;

    /* create a two-column table for all of the widgets */
    GtkTable *table = g_object_new(GTK_TYPE_TABLE,
                       "n-rows", 9,
                       "n-columns", 2,
                       "column-spacing", 6,
                       "row-spacing", 6,
                       NULL);
    gtk_table_attach(table, g_object_new(GTK_TYPE_LABEL,
                                          "label", N_("Input movie file:"),
                                          "use-underline", TRUE,
                                          "mnemonic-widget", file_chooser,
                                          "xalign", 1.0,
                                          "justify", GTK_JUSTIFY_RIGHT,
                                          NULL),
                      0, 1, 0, 1, GTK_EXPAND|GTK_FILL, 0, 0, 0);

    gtk_table_attach(table, g_object_new(GTK_TYPE_ALIGNMENT,
                                          "xalign", 0.0,
                                          "child", file_chooser,
                                          NULL),
                      1, 2, 0, 1, GTK_EXPAND|GTK_FILL, 0, 0, 0);

    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(table), FALSE, FALSE, 0);
}

static void init_statusbar(GtkWidget *window, GtkWidget *vbox, GtranscoderApp *app)
{
    GtkWidget *statusbar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, FALSE, 0);
    app->statusbar = statusbar;
}

void run_main_window(GtranscoderOptions *options)
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Gtranscoder"); // TODO: " - " APP_ONE_LINER);
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
    GtranscoderApp *app = g_new0(GtranscoderApp, 1);
    // FIXME: don't need that:
    g_object_set_data(G_OBJECT(window), APP_DATA_KEY, app);
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

