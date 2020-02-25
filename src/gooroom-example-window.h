/* gooroom-example-window.h
 *
 * Copyright 2020 donghun
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GOOROOM_EXAMPLE_TYPE_WINDOW (gooroom_example_window_get_type())

enum {
    PROCESS_NAME_COLUMN = 0,
    PID_COLUMN,
    USER_COLUMN,
    CPU_COLUMN,
    MEMORY_COLUMN,
    PRIORITY_COLUMN,
    NICE_COLUMN,
    VIRTUAL_MEMORY_COLUMN,
    RESIDENT_MEMORY_COLUMN,
    SHARED_MEMORY_COLUMN,
    STATUS_COLUMN,
    NUM_COLUMNS
};

G_DECLARE_FINAL_TYPE (GooroomExampleWindow, gooroom_example_window, GOOROOM_EXAMPLE, WINDOW, GtkApplicationWindow)

G_END_DECLS
