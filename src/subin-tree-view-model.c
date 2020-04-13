#include "subin-config.h"
#include "model.h"
#include "subin-tree-view-model.h"
#include "subin-view-model-listeners.h"

G_DEFINE_TYPE (SubinTreeViewModel, subin_tree_view_model, GTK_TYPE_LIST_STORE)
void
subin_tree_view_model_clear (SubinTreeViewModel *view_model)
{
    gtk_list_store_clear (GTK_LIST_STORE (view_model));
}


void
subin_tree_view_model_add_columns (GtkTreeView *treeview)
{
    GtkCellRenderer     *renderer;
    GtkTreeViewColumn   *column;

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

    /*Memory*/
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes("Memory",
            renderer,
            "text", MEMORY_COLUMN,
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

#if 1
    static void 
subin_tree_view_model_set (void *data, void *model)
{    
    int i = 0;
    GtkTreeIter iter;
    GPtrArray *sysinfos = data;
    SubinTreeViewModel *view_model = model;

    for (i = 0; i < sysinfos->len; i++)
    {
        gtk_list_store_append (GTK_LIST_STORE (view_model), &iter);

        ProcInfo *info = g_ptr_array_index (sysinfos, i);
        gtk_list_store_set (GTK_LIST_STORE (view_model), &iter,
                PROCESS_NAME_COLUMN,    info->process_name,
                PID_COLUMN,             info->pid,
                USER_COLUMN,            g_strdup        (info->user),
                CPU_COLUMN,             g_strdup_printf ("%d", info->cpu),
                MEMORY_COLUMN,          g_strdup_printf ("%d", info->memory),
                PRIORITY_COLUMN,        g_strdup_printf ("%d", info->priority),
                NICE_COLUMN,            g_strdup_printf ("%d", info->nice),
                VIRTUAL_MEMORY_COLUMN,  g_strdup_printf ("%ld", info->virtual_memory),
                RESIDENT_MEMORY_COLUMN, g_strdup_printf ("%ld", info->resident_memory),
                SHARED_MEMORY_COLUMN,   g_strdup_printf ("%ld", info->shared_memory),
                STATUS_COLUMN,          g_strdup        (info->status),
                -1);
    }
}
#endif

#if 1
static void
subin_tree_view_model_update (void *data, void *model)
{
    int i = 0;

    GtkTreeIter     iter;
    gboolean        found;
    gint            item_pid;

    GPtrArray *sysinfos = data;
    SubinTreeViewModel *view_model = model;


    for (i = 0; i < sysinfos->len; i++)
    {
        found = FALSE;
        if (!gtk_tree_model_get_iter_first (GTK_TREE_MODEL (view_model), &iter))
        {
            subin_tree_view_model_set (data, model);
            break;
        }
        do
        {
            gtk_tree_model_get (GTK_TREE_MODEL (view_model), &iter, PID_COLUMN, &item_pid, -1);
            ProcInfo *info = g_ptr_array_index (sysinfos, i);
            if (info->pid == item_pid)
            {
                found = TRUE;

                gtk_list_store_set (GTK_LIST_STORE (view_model), &iter,
                        PROCESS_NAME_COLUMN,    info->process_name,
                        PID_COLUMN,             info->pid,
                        USER_COLUMN,            g_strdup        (info->user),
                        CPU_COLUMN,             g_strdup_printf ("%d", info->cpu),
                        MEMORY_COLUMN,          g_strdup_printf ("%d", info->memory),
                        PRIORITY_COLUMN,        g_strdup_printf ("%d", info->priority),
                        NICE_COLUMN,            g_strdup_printf ("%d", info->nice),
                        VIRTUAL_MEMORY_COLUMN,  g_strdup_printf ("%ld", info->virtual_memory),
                        RESIDENT_MEMORY_COLUMN, g_strdup_printf ("%ld", info->resident_memory),
                        SHARED_MEMORY_COLUMN,   g_strdup_printf ("%ld", info->shared_memory),
                        STATUS_COLUMN,          g_strdup        (info->status),
                        -1);
            }
        }while (!found && gtk_tree_model_iter_next (GTK_TREE_MODEL (view_model), &iter) );


        if (!found)
        {
            gtk_list_store_append (GTK_LIST_STORE (view_model), &iter);
            ProcInfo *info = g_ptr_array_index (sysinfos, i);
            gtk_list_store_set (GTK_LIST_STORE (view_model), &iter,
                    PROCESS_NAME_COLUMN,    info->process_name,
                    PID_COLUMN,             info->pid,
                    USER_COLUMN,            g_strdup        (info->user),
                    CPU_COLUMN,             g_strdup_printf ("%d", info->cpu),
                    MEMORY_COLUMN,          g_strdup_printf ("%d", info->memory),
                    PRIORITY_COLUMN,        g_strdup_printf ("%d", info->priority),
                    NICE_COLUMN,            g_strdup_printf ("%d", info->nice),
                    VIRTUAL_MEMORY_COLUMN,  g_strdup_printf ("%ld", info->virtual_memory),
                    RESIDENT_MEMORY_COLUMN, g_strdup_printf ("%ld", info->resident_memory),
                    SHARED_MEMORY_COLUMN,   g_strdup_printf ("%ld", info->shared_memory),
                    STATUS_COLUMN,          g_strdup        (info->status),
                    -1);
        }

    }
    gtk_tree_model_get_iter_first (GTK_TREE_MODEL (view_model), &iter);

    do
    {
        gtk_tree_model_get (GTK_TREE_MODEL (view_model), &iter, PID_COLUMN, &item_pid, -1);
        found = FALSE;
        for (i = 0; i < sysinfos->len; i++)
        {
            ProcInfo *info = g_ptr_array_index (sysinfos, i);
            if (info->pid == item_pid)
            {
                found = TRUE;
                break;
            }
        }
        if (!found)
        {
            gtk_list_store_remove (GTK_LIST_STORE (view_model), &iter);
        }
    }while (gtk_tree_model_iter_next (GTK_TREE_MODEL (view_model), &iter));
}
#endif

static void
subin_tree_view_model_dispose (GObject *object)
{
    SubinTreeViewModel *view_model = SUBIN_TREE_VIEW_MODEL (object);
    gtk_list_store_clear (GTK_LIST_STORE (view_model));

    G_OBJECT_CLASS (subin_tree_view_model_parent_class)->dispose (object);
}

static void
subin_tree_view_model_finalize (GObject *object)
{
    G_OBJECT_CLASS (subin_tree_view_model_parent_class)->finalize (object);
}

static void
subin_tree_view_model_class_init (SubinTreeViewModelClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    object_class->dispose = subin_tree_view_model_dispose;
    object_class->finalize = subin_tree_view_model_finalize;
}

static void
subin_tree_view_model_init (SubinTreeViewModel *self)
{
    GType types[NUM_COLUMNS];

    types[PROCESS_NAME_COLUMN] = G_TYPE_STRING; 
    types[PID_COLUMN] = G_TYPE_UINT; 
    types[USER_COLUMN] = G_TYPE_STRING; 
    types[CPU_COLUMN] = G_TYPE_STRING; 
    types[MEMORY_COLUMN] = G_TYPE_STRING; 
    types[PRIORITY_COLUMN] = G_TYPE_STRING; 
    types[NICE_COLUMN] = G_TYPE_STRING; 
    types[VIRTUAL_MEMORY_COLUMN] = G_TYPE_STRING; 
    types[RESIDENT_MEMORY_COLUMN] = G_TYPE_STRING; 
    types[SHARED_MEMORY_COLUMN] = G_TYPE_STRING; 
    types[STATUS_COLUMN] = G_TYPE_STRING; 

    gtk_list_store_set_column_types (GTK_LIST_STORE (self), NUM_COLUMNS, types);

    subin_view_model_listeners_register (EVENT_UPDATE, subin_tree_view_model_update, self);
}

SubinTreeViewModel*
subin_tree_view_model_new (void)
{
    return  g_object_new (SUBIN_TYPE_TREE_VIEW_MODEL, NULL);
}
