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
// FIXME
#include <libgnomeui/libgnomeui.h>
#include "application.h"
#include "config.h"

static const gchar *APP_DATA_KEY = "app-data";

/**
 * Shows an error dialog. The main loop must be running.
 */
GtkWidget *gtranscoder_show_error_dialog(const gchar *message, GtkWidget *parent)
{
    /* TODO: maybe pass a GCallback as well */
    GtkWidget *dialog = gtk_message_dialog_new_with_markup(
        GTK_WINDOW(parent),
        GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_CLOSE,
        message,
        NULL);
    /* Ensure that the dialog box is destroyed when the user responds. */
    g_signal_connect_swapped (dialog,
        "response",
        G_CALLBACK (gtk_widget_destroy),
        dialog);
    gtk_widget_show_all(dialog);
    return dialog;
}

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

static void on_apply_clicked(GtkToolButton *toolbutton, gpointer data)
{
    g_debug("`%s' called", __FUNCTION__);
    GtranscoderApp *app = (GtranscoderApp *) data;
    //app->file_chooser;
    GtkWidget *dialog = gtranscoder_show_error_dialog("The transcoding is not yet implemented.", NULL);
    g_signal_connect_swapped (dialog,
        "response",
        G_CALLBACK (on_quit_chosen), // FIXME
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

static void init_menubar(GtkWidget *window, GtranscoderApp *app)
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

    // FIXME
    gnome_app_set_menus(GNOME_APP(window), GTK_MENU_BAR(menubar));
}


static void init_toolbar(GtkWidget *window, GtranscoderApp *app)
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

    // FIXME
    gnome_app_set_toolbar(GNOME_APP(window), GTK_TOOLBAR(toolbar));
}

static void init_contents(GtkWidget *window, GtranscoderApp *app)
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

    gnome_app_set_contents(GNOME_APP(window), GTK_WIDGET(table));
}

static void init_statusbar(GtkWidget *window, GtranscoderApp *app)
{
    GtkWidget *statusbar = gtk_statusbar_new();
    // FIXME
    gnome_app_set_statusbar(GNOME_APP(window), statusbar);
    app->statusbar = statusbar;
}

void run_main_window()
{
    GtkWidget *window = gnome_app_new(PACKAGE_NAME, "Gtranscoder");
    // FIXME: GnomeApp is deprecated. It should be a window with 4 sections.
    GtranscoderApp *app = g_new0(GtranscoderApp, 1);
    g_object_set_data(G_OBJECT(window), APP_DATA_KEY, app);
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
    g_signal_connect(window, "delete_event", G_CALLBACK(on_delete_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroyed), NULL);

    /* create menu/tool/status bars */
    init_menubar(window, app);
    init_toolbar(window, app);
    init_contents(window, app);
    init_statusbar(window, app);
  
    /* show it all */
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();
}

