/* main.c
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

#include <glib/gi18n.h>

#include "gooroom-example-config.h"
#include "gooroom-example-application.h"

int
main (int   argc,
      char *argv[])
{
    g_autoptr(GtkApplication) app = NULL;

    /* Set up gettext translations */
    bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);

    app = GTK_APPLICATION(gooroom_example_application_new());

    return g_application_run (G_APPLICATION (app), argc, argv);
}
