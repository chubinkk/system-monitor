/* subin-application.c
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

#include "subin-config.h"
#include "subin-window.h"
#include "subin-application.h"

struct _SubinApplicationPrivate
{
    GtkWindow *window;
};

G_DEFINE_TYPE_WITH_PRIVATE (SubinApplication, subin_application, GTK_TYPE_APPLICATION)

static void
quit_activated (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       app)
{
  g_application_quit (G_APPLICATION (app));
}

static GActionEntry app_entries[] =
{
  { "quit", quit_activated, NULL, NULL, NULL }
};

#if 0
static void
search_text_changed (GtkEditable *entry, gpointer data)
{
    GtkTreeView         *active_tree;

    gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER (gtk_tree_model_sort_get_model (
                                    GTK_TREE_MODEL_SORT (gtk_tree_view_get_model(
                                    GTK_TREE_VIEW (active_tree))))));
}


static gboolean
cb_window_key_press_event (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{   
    GtkStack *stack;
    const char *current_page = gtk_stack_get_visible_child_name (stack);  
    if (strcmp (current_page, "active_process") == 0)
        return gtk_search_bar_handle_event (GTK_SEARCH_BAR (user_data), event);
    
    return FALSE;
} 

static void
create_proc_view(GApplication *app, GtkBuilder * builder)
{
    GtkSearchBar        *active_searchbar;
    GtkSearchEntry      *active_searchentry;
    GtkButton           *search_btn;

    SubinApplicationPrivate *priv;
    priv = subin_application_get_instance_private (SUBIN_APPLICATION(app));

    priv->window = gtk_application_get_active_window (GTK_APPLICATION(app));

    /* create popup_menu for the processes tab */
    active_searchbar = GTK_SEARCH_BAR (gtk_builder_get_object (builder, "proc_searchbar"));
    active_searchentry = GTK_SEARCH_ENTRY (gtk_builder_get_object (builder, "proc_searchentry"));

    gtk_search_bar_connect_entry (active_searchbar, GTK_ENTRY (active_searchentry));
    g_signal_connect (priv->window, "key-press-event",
                      G_CALLBACK (cb_window_key_press_event), active_searchbar);

    g_signal_connect (active_searchentry, "changed", G_CALLBACK (search_text_changed), app);

    g_object_bind_property (active_searchbar, "search-mode-enabled", search_btn, "active", (GBindingFlags)(G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE));
}
#endif

static void
subin_application_startup (GApplication *app)
{
  GtkBuilder *builder;
  GMenuModel *app_menu;

  G_APPLICATION_CLASS (subin_application_parent_class)->startup (app);

  g_action_map_add_action_entries (G_ACTION_MAP (app),
                                   app_entries, G_N_ELEMENTS (app_entries),
                                   app);

  builder = gtk_builder_new_from_resource ("/kr/gooroom/Gooroom-Example/subin-menu.ui");
  app_menu = G_MENU_MODEL (gtk_builder_get_object (builder, "appmenu"));
  gtk_application_set_app_menu (GTK_APPLICATION (app), app_menu);
  g_object_unref (builder);
}

static void
subin_application_activate (GApplication *app)
{
    SubinApplicationPrivate *priv;
    priv = subin_application_get_instance_private (SUBIN_APPLICATION(app));

    /* Get the current window or create one if necessary. */
    priv->window = gtk_application_get_active_window (GTK_APPLICATION(app));
    if (priv->window == NULL)
        priv->window = g_object_new (SUBIN_TYPE_WINDOW,
                               "application", app,
                               "default-width", 830,
                               "default-height", 600,
                               NULL);

    /* Ask the window manager/compositor to present the window. */
    gtk_window_present (priv->window);
}

static void
subin_application_finalize (GObject *object)
{
    G_OBJECT_CLASS (subin_application_parent_class)->finalize (object);

    SubinApplication *app = SUBIN_APPLICATION(object);
    SubinApplicationPrivate *priv = app->priv;

    if (priv && priv->window != NULL)
    {
        gtk_widget_destroy (GTK_WIDGET(priv->window));
    }
}

static void
subin_application_init (SubinApplication *application)
{
    SubinApplicationPrivate *priv;
    priv = subin_application_get_instance_private (application);
    priv->window = NULL;
}

static void
subin_application_class_init (SubinApplicationClass *class)
{
    G_OBJECT_CLASS (class)->finalize = subin_application_finalize;
    G_APPLICATION_CLASS (class)->activate = subin_application_activate;
    G_APPLICATION_CLASS (class)->startup = subin_application_startup;
}

SubinApplication*
subin_application_new (void)
{
  return g_object_new (SUBIN_TYPE_APPLICATION,
                       "application-id", "kr.gooroom.Gooroom-Example",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
}
