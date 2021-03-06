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

#include "config.h"
#include "application.h"
#include "transcoding.h"
#include "gtranscoder-error.h"
#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
    
    g_set_application_name (_("Gtranscoder"));

    GtranscoderOptions *options = g_new0(GtranscoderOptions, 1);;
    GOptionEntry entries[] =
    {
        { "verbose", 'v', 0, G_OPTION_ARG_NONE, &options->verbose, N_("Print debug info"), NULL },
        { "version", 'V', 0, G_OPTION_ARG_NONE, &options->show_version, N_("Show version number and exit"), NULL },
        { NULL, 0, 0, G_OPTION_ARG_NONE, NULL, NULL, 0 }
    };

    GError *error = NULL;
    GOptionContext *context;
  
    context = g_option_context_new(N_("- Movie transcoding program"));
    g_option_context_add_main_entries(context, entries, GETTEXT_PACKAGE);
    g_option_context_add_group (context, gtk_get_option_group (TRUE));
    if (g_option_context_parse(context, &argc, &argv, &error) == FALSE)
    {
        gchar *help_text = g_option_context_get_help (context, TRUE, NULL);
        g_print ("%s\n\n%s", error->message, help_text);
        g_free (help_text);
        g_error_free (error);
        g_option_context_free (context);
        g_free(options);
        return 1;
    }
    if (options->show_version)
    {
        g_print("Gtranscoder " VERSION " \n");
        return 0;
    }

    g_option_context_free (context);
    gtk_init(&argc, &argv);
    
    gboolean has_ffmpeg = check_for_ffmpeg();
    if (! has_ffmpeg)
    {
        gchar *error_mess = N_("Gtranscoder could not find the ffmpeg executable. Aborting.");
        g_critical("%s", error_mess);
        gtranscoder_show_error_dialog_without_main_loop(error_mess);
        g_error("%s", N_("ffmpeg is required."));
        return 1;
    }
    else
    {
        g_debug("%s", N_("Found ffmpeg"));
        run_main_window(options);
        gtk_main();
    }
    g_free(options);
    return 0;
}

