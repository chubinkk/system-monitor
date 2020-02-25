/* gooroom-example-application.h
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

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GOOROOM_EXAMPLE_TYPE_APPLICATION (gooroom_example_application_get_type ())
#define GOOROOM_EXAMPLE_APPLICATION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GOOROOM_EXAMPLE_TYPE_APPLICATION, GooroomExampleApplication))

typedef struct _GooroomExampleApplication           GooroomExampleApplication;
typedef struct _GooroomExampleApplicationClass      GooroomExampleApplicationClass;
typedef struct _GooroomExampleApplicationPrivate    GooroomExampleApplicationPrivate;

struct _GooroomExampleApplication
{
  GtkApplication parent;
  GooroomExampleApplicationPrivate *priv;
};

struct _GooroomExampleApplicationClass
{
  GtkApplicationClass parent_class;
};

GType            			gooroom_example_application_get_type        (void);
GooroomExampleApplication  *gooroom_example_application_new             (void);

G_END_DECLS
