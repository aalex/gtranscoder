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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * Runs a command synchronously;
 * Returns its success value.
 */
gboolean run_command(const gchar *command)
{
    GError *error = NULL;
    gint status = 0;
    gchar *stdout_message = NULL;
    gchar *stderr_message = NULL;
    gboolean ret = FALSE;
    
    ret = g_spawn_command_line_sync(command, &stdout_message, &stderr_message, &status, &error);
    if ((error && (error->code != 0)) || ! WIFEXITED(status) || WEXITSTATUS(status))
        g_warning("Failed to execute command \"%s\", exited: %i, status: %i, stderr: %s, stdout: %s\n", command, WIFEXITED(status), WEXITSTATUS(status), stderr_message, stdout_message);
    else
        ret = TRUE;
    if (error)
        g_error_free(error);
    g_free(stdout_message);
    g_free(stderr_message);
    return ret;
}

gboolean check_for_ffmpeg()
{
    gchar *command = "ffmpeg -version";
    gboolean ret = run_command(command);
    return ret;
}

