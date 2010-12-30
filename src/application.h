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

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <gtk/gtk.h>

typedef struct options_
{
    gboolean verbose;
} GtranscoderOptions;

typedef struct application_
{
    GtkWidget *file_chooser;
    GtkWidget *statusbar;
} GtranscoderApp;

void run_main_window();

#endif

