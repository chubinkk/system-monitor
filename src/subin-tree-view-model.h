#ifndef SUBIN_TREE_VIEW_MODEL_H
#define SUBIN_TREE_VIEW_MODEL_H

#include <gtk/gtk.h>
#include <glib-object.h>

#define SUBIN_TYPE_TREE_VIEW_MODEL (subin_tree_view_model_get_type ())
#define SUBIN_TREE_VIEW_MODEL(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SUBIN_TYPE_TREE_VIEW_MODEL, SubinTreeViewModel))

typedef struct _SubinTreeViewModel SubinTreeViewModel;
typedef struct _SubinTreeViewModelClass SubinTreeViewModelClass;

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

struct _SubinTreeViewModel
{
    GtkListStore    parent_instance;
};

struct _SubinTreeViewModelClass
{
    GtkListStoreClass parent_class;
};

void                subin_tree_view_model_add_columns (GtkTreeView *treeview);
void                subin_tree_view_model_selected_row (GtkTreeView *treeview);
void                subin_tree_view_model_clear (SubinTreeViewModel *view_model);

GType               subin_tree_view_model_get_type (void);
SubinTreeViewModel *subin_tree_view_model_new (void);

#endif /* __SUBIN_TREE_VIEW_MODLE_H */

