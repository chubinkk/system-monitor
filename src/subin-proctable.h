/* subin-proctable.h
 *
 * Copyright 2020 subin 
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
typedef struct
{
    const gchar     *process_name;
    pid_t            pid;
    const gchar     *user;
    const gint       cpu;
    guint64          memory;
    const gchar     *priority;
    const gchar     *nice;
    const gchar     *virtual_memory;
    const gchar     *resident_memory;
    const gchar     *shared_memory;
    const gchar     *status;
}ProcInfo;

void get_process_info (ProcInfo info[]);

G_END_DECLS

