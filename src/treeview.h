/* treeview.h
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

#define TYPE_TREE_VIEW (tree_view_get_type ())
#define TREE_VIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_TREE_VIEW, TreeView))
#define TREE_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_TREE_VIEW, TreeViewClass))

#define IS_TREE_VIEW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_TREE_VIEW))
#define IS_TREE_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_TREE_VIEW))
#define _TYPE_WINDOW (window_get_type())

typedef struct _TreeView           TreeView;
typedef struct _TreeViewClass      TreeViewClass;
typedef struct _TreeViewClass      TreeViewClass;


struct _TreeView
{
    GtkTreeView parent_instance;
};

struct _TreeViewClass
{
    GtkTreeViewClass parent_class;
};

GType                   tree_view_get_type               (void) G_GNUC_CONST;
TreeView              * tree_view_new                    (GSettings      *settings,
                                                          gboolean        store_column_order);
void                    tree_view_save_state             (TreeView       *tree_view);
void                    tree_view_load_state             (TreeView       *tree_view);
GtkTreeViewColumn     * tree_view_get_column_from_id     (TreeView       *tree_view,
                                                          gint            sort_id);
void                    tree_view_add_excluded_column    (TreeView       *tree_view,
                                                          gint            column_id);
void                    tree_view_append_and_bind_column (TreeView            *tree_view,
                                                          GtkTreeViewColumn   *column);
G_END_DECLS

