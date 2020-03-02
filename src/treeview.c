/* treeview.c
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

#include "treeview.h"
#include "subin-proctable.h"

typedef struct
{
    GSettings  *settings;
    gboolean    store_column_order;
    GHashTable *excluded_columns;
} TreeViewPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (TreeView, tree_view, GTK_TYPE_TREE_VIEW)

//-----------------------------------------------------------------------------------------//

static void
tree_view_finalize (GObject *object)
{
    TreeViewPrivate *priv = tree_view_get_instance_private (TREE_VIEW (object));

    g_hash_table_destroy (priv->excluded_columns);
    priv->excluded_columns = NULL;

//    G_OBJECT_CLASS (tree_view_parent_class)->tree_view_finalize (object);
}

static void
tree_view_init (TreeView *self)
{
    TreeViewPrivate *priv = tree_view_get_instance_private (self);

    priv->excluded_columns = g_hash_table_new (g_direct_hash, g_direct_equal);
}

static void
tree_view_class_init (TreeViewClass *klass)
{
#if 0
    GObjectClass *oclass = G_OBJECT_CLASS (klass);
    oclass->dispose = tree_view_dispose;
    oclass->finalize = tree_view_finalize;
    oclass->constructed = tree_view_constructed;
    
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);   
    widget_class->destroy = destroy;
    widget_class->show = show;
    widget_class->realize = realize;
    widget_class->key_press_event = key_press_event;
    widget_class->delete_event = delete_event;
    widget_class->grab_focus = grab_focus;
#endif

    GObjectClass *o_class = G_OBJECT_CLASS (klass);

    o_class->finalize = tree_view_finalize;
}

//-----------------------------------------------------------------------------------------//
void
tree_view_save_state (TreeView *tree_view)
{
    TreeViewPrivate *priv;

    g_return_if_fail (IS_TREE_VIEW (tree_view));

    priv = tree_view_get_instance_private (tree_view);
    GtkTreeModel *model;
    gint sort_col;
    GtkSortType sort_type;

    model = gtk_tree_view_get_model (GTK_TREE_VIEW (tree_view));

    if (gtk_tree_sortable_get_sort_column_id (GTK_TREE_SORTABLE (model),
                                              &sort_col,
                                              &sort_type)) {
        g_settings_set_int (priv->settings, "sort-col", sort_col);
        g_settings_set_int (priv->settings, "sort-order", sort_type);
    }

    if (priv->store_column_order) {
        GList *columns = gtk_tree_view_get_columns (GTK_TREE_VIEW (tree_view));
        GList *iter;
        GVariantBuilder builder;

        g_variant_builder_init (&builder, G_VARIANT_TYPE_ARRAY);

        for (iter = columns; iter != NULL; iter = iter->next) {
            gint id = gtk_tree_view_column_get_sort_column_id (GTK_TREE_VIEW_COLUMN (iter->data));
            g_variant_builder_add (&builder, "i", id);
        }

        g_settings_set_value (priv->settings, "columns-order",
                              g_variant_builder_end (&builder));

        g_list_free (columns);
    }
}

GtkTreeViewColumn *
tree_view_get_column_from_id (TreeView *tree_view, gint sort_id)
{
    GList *columns;
    GList *iter;
    GtkTreeViewColumn *col = NULL;

    g_return_val_if_fail (IS_TREE_VIEW (tree_view), NULL);

    columns = gtk_tree_view_get_columns (GTK_TREE_VIEW (tree_view));

    for (iter = columns; iter != NULL; iter = iter->next) {
        col = GTK_TREE_VIEW_COLUMN (iter->data);
        if (gtk_tree_view_column_get_sort_column_id (col) == sort_id)
            break;
    }

    g_list_free (columns);

    return col;
}

static gboolean
cb_column_header_clicked (GtkTreeViewColumn *column, GdkEventButton *event, gpointer data)
{
    GtkMenu *menu = GTK_MENU (data);

    if (event->button == GDK_BUTTON_SECONDARY) {
        gtk_menu_popup_at_pointer (menu, (GdkEvent*)event);
        return TRUE;
    }

    return FALSE;
}

void
tree_view_load_state (TreeView *tree_view)
{
    TreeViewPrivate *priv;
    GtkTreeModel *model;
    gint sort_col;
    GtkSortType sort_type;

    g_return_if_fail (IS_TREE_VIEW (tree_view));

    priv = tree_view_get_instance_private (tree_view);
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (tree_view));

    sort_col = g_settings_get_int (priv->settings, "sort-col");
    sort_type = g_settings_get_int (priv->settings, "sort-order");

    gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (model),
                                          sort_col,
                                          sort_type);

    if (priv->store_column_order) {
        GtkMenu *header_menu = GTK_MENU (gtk_menu_new ());
        GList *columns = gtk_tree_view_get_columns (GTK_TREE_VIEW (tree_view));
        GList *iter;
        GVariantIter *var_iter;
        GtkTreeViewColumn *col, *last;
        gint sort_id;

        for (iter = columns; iter != NULL; iter = iter->next) {
            const char *title;
            char *key;
            GtkButton *button;
            GtkCheckMenuItem *column_item;

            col = GTK_TREE_VIEW_COLUMN (iter->data);
            sort_id = gtk_tree_view_column_get_sort_column_id (col);

            if (priv->excluded_columns &&
                g_hash_table_contains (priv->excluded_columns, GINT_TO_POINTER (sort_id))) {
                gtk_tree_view_column_set_visible (col, FALSE);
                continue;
            }

            title = gtk_tree_view_column_get_title (col);

            button = GTK_BUTTON (gtk_tree_view_column_get_button (col));
            g_signal_connect (button, "button-press-event",
                              G_CALLBACK (cb_column_header_clicked),
                              header_menu);

            column_item = GTK_CHECK_MENU_ITEM (gtk_check_menu_item_new_with_label (title));
            g_object_bind_property (col, "visible",
                                    column_item, "active",
                                    G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);

            gtk_menu_shell_append (GTK_MENU_SHELL (header_menu), GTK_WIDGET (column_item));

            key = g_strdup_printf ("col-%d-width", sort_id);
            gtk_tree_view_column_set_fixed_width (col, g_settings_get_int (priv->settings, key));
            gtk_tree_view_column_set_min_width (col, 30);
            g_free (key);

            key = g_strdup_printf ("col-%d-visible", sort_id);
            gtk_tree_view_column_set_visible (col, g_settings_get_boolean (priv->settings, key));
            g_free (key);
        }

        g_list_free (columns);

        gtk_widget_show_all (GTK_WIDGET (header_menu));

        g_settings_get (priv->settings, "columns-order", "ai", &var_iter);
        last = NULL;
        while (g_variant_iter_loop (var_iter, "i", &sort_id)) {
            col = tree_view_get_column_from_id (tree_view, sort_id);

            if (col != NULL && col != last) {
                gtk_tree_view_move_column_after (GTK_TREE_VIEW (tree_view),
                                                 col, last);
                last = col;
            }
        }
        g_variant_iter_free (var_iter);
    }
}

void
tree_view_add_excluded_column (TreeView *tree_view, gint column_id)
{
    TreeViewPrivate *priv;

    g_return_if_fail (IS_TREE_VIEW (tree_view));

    priv = tree_view_get_instance_private (tree_view);
    g_hash_table_add (priv->excluded_columns, GINT_TO_POINTER (column_id));
}

static guint timeout_id = 0;
static GtkTreeViewColumn *current_column;

static gboolean
save_column_state (gpointer data)
{
    GSettings *settings = G_SETTINGS (data);
    gint column_id = gtk_tree_view_column_get_sort_column_id (current_column);
    gint width = gtk_tree_view_column_get_width (current_column);
    gboolean visible = gtk_tree_view_column_get_visible (current_column);

    gchar *key;

    key = g_strdup_printf ("col-%d-width", column_id);
    g_settings_set_int (settings, key, width);
    g_free (key);

    key = g_strdup_printf ("col-%d-visible", column_id);
    g_settings_set_boolean (settings, key, visible);
    g_free (key);
    timeout_id = 0;
    return FALSE;
}

static void
cb_update_column_state (GObject *object, GParamSpec *pspec, gpointer data)
{
    GtkTreeViewColumn *column = GTK_TREE_VIEW_COLUMN (object);

    current_column = column;

    if (timeout_id > 0)
        g_source_remove (timeout_id);

    timeout_id = g_timeout_add_seconds (1, save_column_state, data);
}

void
tree_view_append_and_bind_column (TreeView *tree_view, GtkTreeViewColumn *column)
{
    TreeViewPrivate *priv;

    g_return_if_fail (IS_TREE_VIEW (tree_view));
    g_return_if_fail (GTK_IS_TREE_VIEW_COLUMN (column));

    priv = tree_view_get_instance_private (tree_view);

    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view),
                                 column);

    g_signal_connect (column, "notify::fixed-width",
                      G_CALLBACK (cb_update_column_state),
                      priv->settings);

    g_signal_connect (column, "notify::visible",
                      G_CALLBACK (cb_update_column_state),
                      priv->settings);
}

TreeView *
tree_view_new (GSettings *settings, gboolean store_column_order)
{
    TreeView *self = g_object_new (TYPE_TREE_VIEW, NULL);
//-----------------------------------------------------------------------------------------//
    TreeViewPrivate *priv = tree_view_get_instance_private (self);

    priv->settings = settings;
    priv->store_column_order = store_column_order;

    return self;
}

//-----------------------------------------------------------------------------------------//

static GtkTreeModel *
create_model (void)
{
    GtkTreeStore *store;
    GtkTreeIter iter;
    
    ProcInfo info[4096] = {0, };
    int info_size = 0;
    int i = 0;

    info_size = get_process_info(&info);

    store = gtk_tree_store_new (NUM_COLUMNS,
                                G_TYPE_STRING,
                                G_TYPE_STRING,
                                G_TYPE_STRING,
                                G_TYPE_STRING,
                                G_TYPE_STRING,
                                G_TYPE_STRING,
                                G_TYPE_STRING,
                                G_TYPE_STRING,
                                G_TYPE_STRING,
                                G_TYPE_STRING,
                                G_TYPE_STRING,
                                G_TYPE_STRING,
                                G_TYPE_STRING);
    /* add data to the tree store */
    for (i = 0; i <info_size; i++)
    {
        gtk_tree_store_append (store, &iter, NULL);
        /*
        gtk_tree_store_set (store, &iter,
                            COLUMN_PID,   info[i].pid,
                            COLUMN_NAME,  info[i].name,
                            COLUMN_USER,  info[i].user,
                            COLUMN_STATE, info[i].state,
                            COLUMN_CPU,   info[i].cpu,
                            COLUMN_MEM,   info[i].mem,
                            -1
                            );
    */

        gtk_tree_store_set(store, &iter, PID_COLUMN, info[i].pid,-1);
        gtk_tree_store_set(store, &iter, PROCESS_NAME_COLUMN, info[i].process_name,-1);
        gtk_tree_store_set(store, &iter, USER_COLUMN, info[i].user,-1);
        gtk_tree_store_set(store, &iter, STATUS_COLUMN, info[i].status,-1);
        gtk_tree_store_set(store, &iter, CPU_COLUMN, info[i].cpu,-1);
        gtk_tree_store_set(store, &iter, MEMORY_COLUMN, info[i].memory,-1);
    }

//    gtk_tree_store_append (store, &iter, NULL);

//    gtk_tree_store_set (store, &iter,
//                        PROCESS_NAME_COLUMN, "name1",
//                        PID_COLUMN, "pid1",
//                        USER_COLUMN, "User1",
//                        CPU_COLUMN, "cpu",
//                        MEMORY_COLUMN, "mem",
//                        PRIORITY_COLUMN, "Priority1",
//                        NICE_COLUMN, "nice",
//                        VIRTUAL_MEMORY_COLUMN, "virt",
//                        RESIDENT_MEMORY_COLUMN, "resident",
//                        SHARED_MEMORY_COLUMN, "shared",
//                        STATUS_COLUMN,"status");

    return GTK_TREE_MODEL (store);
}

static void
add_columns (GtkTreeView *treeview)
{
    gint i = 0;

    GtkCellRenderer     *renderer;
    GtkTreeViewColumn   *column;
    GtkTreeModel *model = gtk_tree_view_get_model (treeview);

    /*Process Name*/
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes("Process Name",
                                                       renderer,
                                                      "text", PROCESS_NAME_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (treeview, column);

    /*PID*/
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes("PID",
                                                       renderer,
                                                      "text", PID_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (treeview, column);

    /*User*/
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes("User",
                                                       renderer,
                                                      "text", USER_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (treeview, column);

    /*CPU*/
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes("CPU",
                                                       renderer,
                                                      "text", CPU_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (treeview, column);

    /*Priority*/
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes("Priority",
                                                       renderer,
                                                      "text", PRIORITY_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (treeview, column);

    /*Nice*/
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes("Nice",
                                                       renderer,
                                                      "text", NICE_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (treeview, column);

    /*Virtual Memory*/
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes("Virtual Memory",
                                                       renderer,
                                                      "text", VIRTUAL_MEMORY_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (treeview, column);

    /*Resident Memory*/
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes("Resident Memory",
                                                       renderer,
                                                      "text", RESIDENT_MEMORY_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (treeview, column);

    /*Shared Memory*/
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes("Shared Memory",
                                                       renderer,
                                                      "text", SHARED_MEMORY_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (treeview, column);

    /*Status*/
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes("Status",
                                                       renderer,
                                                      "text", STATUS_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (treeview, column);
}
