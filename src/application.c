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
    gtk_toolbar_insert_stock(toolbar, GTK_STOCK_APPLY,
                           N_("Proceed with the movie transcoding"),
                           NULL, G_CALLBACK(on_apply_clicked), N_("Apply"), -1);

    /* the contents */
    GtkWidget *label = gtk_label_new(N_("Hello"));

    gnome_app_set_contents(GNOME_APP(window), label);
    gnome_app_set_menus(GNOME_APP(window), GTK_MENU_BAR(menubar));
    gnome_app_set_toolbar(GNOME_APP(window), GTK_TOOLBAR(toolbar));
    gnome_app_set_statusbar(GNOME_APP(window), statusbar);
  
    /* show it all */
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();
}

