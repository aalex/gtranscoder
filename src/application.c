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

#include "application.h"
#include "config.h"
#include <gnome.h>
#include <libgnomeui/libgnomeui.h>

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

static void on_apply_clicked(GtkWidget *item, gpointer data)
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

/* handler for GnomeFileEntry::activate */
void fileentry_activated(GnomeFileEntry *file_widget, gpointer data)
{
    g_debug("`%s' called", __FUNCTION__);
    gchar *path;
    path = gnome_file_entry_get_full_path(file_widget, FALSE);
    g_debug("File chooser activated; new File: %s", path);
    g_free(path);
}

void run_main_window()
{
    GtkWidget *window = gnome_app_new(PACKAGE_NAME, "Gtranscoder");
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 640);

    g_signal_connect(window, "delete_event", G_CALLBACK(on_delete_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroyed), NULL);

    /* setup shortcuts */
    GtkAccelGroup *accel_group = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);
  
    /* create menu/tool/status bars */
    GtkWidget *menubar = gtk_menu_bar_new();
    GtkWidget *toolbar = gtk_toolbar_new();
    GtkWidget *statusbar = gtk_statusbar_new();

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

    /* the toolbar buttons */
    GtkToolItem *apply_tool = gtk_tool_button_new_from_stock(GTK_STOCK_APPLY);
    //gtk_tool_button_set_label(GTK_TOOL_BUTTON(apply_tool), N_("Apply"));
    //gtk_widget_set_tooltip_text(GTK_WIDGET(apply_tool), N_("Proceed with the movie transcoding"));
    gtk_toolbar_insert(toolbar, apply_tool, -1);

    /* file chooser */
    GtkWidget *file_chooser = g_object_new(GNOME_TYPE_FILE_ENTRY, "history-id", "fileentry",
                            NULL);
    g_signal_connect(file_chooser, "activate", G_CALLBACK(fileentry_activated),
                   NULL);

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
    gnome_app_set_menus(GNOME_APP(window), GTK_MENU_BAR(menubar));
    gnome_app_set_toolbar(GNOME_APP(window), GTK_TOOLBAR(toolbar));
    gnome_app_set_statusbar(GNOME_APP(window), statusbar);
  
    /* show it all */
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();
}

