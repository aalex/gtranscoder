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

#include "gtranscoder-error.h"
#include <gtk/gtk.h>

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

void gtranscoder_show_error_dialog_without_main_loop(gchar *message)
{
    GtkWidget *dialog = gtranscoder_show_error_dialog(message, NULL);
    gtk_dialog_run(GTK_DIALOG(dialog));
    g_debug("Done showing the error dialog.");
}

