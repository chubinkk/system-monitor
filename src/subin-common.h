/* subin-common.h
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


#ifndef SUBIN_COMMON_H_
#define SUBIN_COMMON_H

#include <gtk/gtk.h>

typedef struct
{
    guint  id;
	GPtrArray *sysinfos;
}COMMON_INFO;

void set_page   (gchar *page_name);
//void set_flag   (int flag);
void start_job  ();
void end_job    ();

#endif /* __SUBIN_COMMON_H*/
