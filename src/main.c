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
#include <gnome.h>

int main(int argc, char *argv[])
{
    GnomeProgram *program;
    /* initialize GNOME */
    program = gnome_program_init(PACKAGE,
        PACKAGE_VERSION,
        LIBGNOMEUI_MODULE,
        argc, argv,
        GNOME_PROGRAM_STANDARD_PROPERTIES,
        GNOME_PARAM_HUMAN_READABLE_NAME, "Gtranscoder",
        GNOME_PARAM_ENABLE_SOUND, TRUE,
        NULL);
    
    gboolean has_ffmpeg = check_for_ffmpeg();
    if (! has_ffmpeg)
    {
        g_critical("%s", N_("Could not find the ffmpeg executable."));
        return 1;
    }
    else
    {
        g_debug("%s", N_("Found ffmpeg"));
        run_main_window();
    }
    return 0;
}

