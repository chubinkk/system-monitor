/* subin-application.h
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

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SUBIN_TYPE_APPLICATION (subin_application_get_type ())
#define SUBIN_APPLICATION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SUBIN_TYPE_APPLICATION, SubinApplication))

typedef struct _SubinApplication           SubinApplication;
typedef struct _SubinApplicationClass      SubinApplicationClass;
typedef struct _SubinApplicationPrivate    SubinApplicationPrivate;

struct _SubinApplication
{
  GtkApplication parent;
  SubinApplicationPrivate *priv;
};

struct _SubinApplicationClass
{
  GtkApplicationClass parent_class;
};

GType            			subin_application_get_type        (void);
SubinApplication           *subin_application_new             (void);

G_END_DECLS
