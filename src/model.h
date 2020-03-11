/* model.h
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

#if 1 
typedef enum
{
    PROCESS_VIEW_ACTIVE,
    PROCESS_VIEW_ALL,
    PROCESS_VIEW_MY
}ProcessView;
#endif

typedef struct _ProcInfo
{
    gchar     *process_name;
    pid_t      pid;
    gchar     *user;
    gint32     uid;
    gint32     cpu;
    gint32     memory;
    gint32     priority;
    gint32     nice;
    guint64    virtual_memory;
    guint64    resident_memory;
    guint64    shared_memory;
    gchar     *status;
}ProcInfo;





