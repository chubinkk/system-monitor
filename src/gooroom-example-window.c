/* gooroom-example-window.c
 *
 * Copyright 2020 donghun->subin
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

#include <glibtop/cpu.h>
#include <glibtop/proclist.h>
#include <glibtop/procstate.h>
#include <glibtop/procuid.h>


#include "gooroom-example-config.h"
#include "gooroom-example-window.h"

struct _GooroomExampleWindow
{
    GtkApplicationWindow  parent_instance;

    /* Template widgets */
    GtkHeaderBar        *header_bar;
    GtkToggleButton     *search_btn;
    GtkButton           *terminate_btn;       
    GtkButton           *stop_btn;
    GtkButton           *continue_btn;
    GtkButton           *kill_btn;

    GtkTreeView         *active_tree;
    GtkTreeView         *all_tree;
    GtkTreeView         *my_tree;
};

G_DEFINE_TYPE (GooroomExampleWindow, gooroom_example_window, GTK_TYPE_APPLICATION_WINDOW)

//struct ProcInfo
//{
//    const gchar     *process_name;
//    const gchar     *pid;
//    const gchar     *user;
//    const gchar     *cpu;
//    const gchar     *memory;
//    const gchar     *priority;
//    const gchar     *nice;
//    const gchar     *virtual_memory;
//    const gchar     *resident_memory;
//    const gchar     *shared_memory;
//    const gchar     *status;
//};

static void
get_process_info(void)
{
    pid_t*              pid_list;
    glibtop_cpu         cpu;
    glibtop_proclist    proclist;
    glibtop_proc_state  procstate;
    glibtop_proc_uid    procuid;
    g_autofree gchar   *pid;

    gint i;
    gint arg = 0;
    GError *error = NULL;

    glibtop_get_cpu (&cpu);
    pid_list = glibtop_get_proclist (&proclist, GLIBTOP_KERN_PROC_ALL, arg);
    glibtop_get_proc_state (&procstate, arg);

    pid = g_strdup_printf ("task total: %ld\n", proclist.number);

    for (i = 0; i < proclist.number; i++)
        g_print ("pid[%d]: %d\n",i, pid_list[i]);

/*-------------------------------------------------------------------------------------------*/
//    g_strdup_printf ("Total %ld , User:%ld\n", cpu.total, cpu.user);
//    g_strdup_printf ("Proclist number: %ld , Proclist total: %ld , Proclist size: %ld\n", proclist.number, proclist.total, proclist.size);
//    g_strdup_printf ("Proc_state: %s\n", procstate.cmd);
//    glibtop_get_proc_uid (pid, pid_list);
/*-------------------------------------------------------------------------------------------*/

}

static GtkTreeModel *
create_model (void)
{
    GtkTreeStore *store;
    GtkTreeIter iter;

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

    gtk_tree_store_append (store, &iter, NULL);
    
    gtk_tree_store_set (store, &iter,
                        PROCESS_NAME_COLUMN, "name1",
                        PID_COLUMN, "pid1",
                        USER_COLUMN, "User1",
                        CPU_COLUMN, "cpu",
                        MEMORY_COLUMN, "mem",      
                        PRIORITY_COLUMN, "Priority1",
                        NICE_COLUMN, "nice",
                        VIRTUAL_MEMORY_COLUMN, "virt",
                        RESIDENT_MEMORY_COLUMN, "resident",
                        SHARED_MEMORY_COLUMN, "shared",
                        STATUS_COLUMN,"status");

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

static void
gooroom_example_window_constructed (GObject *obj) 
{
    G_OBJECT_CLASS(gooroom_example_window_parent_class) -> constructed(obj);
    GooroomExampleWindow *win = GOOROOM_EXAMPLE_WINDOW(obj);    
    
    GtkTreeStore    *store;
    GtkTreeIter     iter;
    GtkTreeModel    *model;

    model = create_model();
    gtk_tree_view_set_model (win->active_tree, model);
    add_columns (GTK_TREE_VIEW (win->active_tree));

    model = create_model();
    gtk_tree_view_set_model (win->all_tree, model);
    add_columns (GTK_TREE_VIEW (win->all_tree));

    model = create_model();
    gtk_tree_view_set_model (win->my_tree, model);
    add_columns (GTK_TREE_VIEW (win->my_tree));

    g_object_unref (model);

    get_process_info();
}

static void
terminate_clicked (GtkButton *button, GooroomExampleWindow *win)
{
    g_print ("= = = = = = => terminate button clicked \n");
}

static void
stop_clicked (GtkButton *button, GooroomExampleWindow *win) 
{
    g_print ("= = = = = = => stop button clicked \n");
}

static void
continue_clicked (GtkButton *button, GooroomExampleWindow *win)
{
    g_print ("= = = = = = => continue button clicked \n");
}

static void
kill_clicked (GtkButton *button, GooroomExampleWindow *win)
{
    g_print ("= = = = = = => kill button clicked \n");
}

static void
gooroom_example_window_init (GooroomExampleWindow *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));
    gtk_widget_show_all (GTK_WIDGET (self));

}

static void
gooroom_example_window_class_init (GooroomExampleWindowClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
#if 0
    GObjectClass *oclass = G_OBJECT_CLASS (klass);
    oclass->dispose = gooroom_example_window_dispose;
    oclass->finalize = gooroom_example_window_finalize;
    oclass->constructed = gooroom_example_window_constructed;
    
	widget_class->destroy = gooroom_example_window_destroy;
    widget_class->show = gooroom_example_window_show;
    widget_class->realize = gooroom_example_window_realize;
    widget_class->key_press_event = gooroom_example_window_key_press_event;
    widget_class->delete_event = gooroom_example_window_delete_event;
    widget_class->grab_focus = gooroom_example_window_grab_focus;
#endif

    GObjectClass *oclass = G_OBJECT_CLASS (klass);
    oclass->constructed = gooroom_example_window_constructed;
    
    gtk_widget_class_set_template_from_resource (widget_class, "/kr/gooroom/Gooroom-Example/gooroom-example-window.ui");
    gtk_widget_class_bind_template_child (widget_class, GooroomExampleWindow, header_bar);
    gtk_widget_class_bind_template_child (widget_class, GooroomExampleWindow, search_btn);
    gtk_widget_class_bind_template_child (widget_class, GooroomExampleWindow, terminate_btn);
    gtk_widget_class_bind_template_child (widget_class, GooroomExampleWindow, stop_btn);
    gtk_widget_class_bind_template_child (widget_class, GooroomExampleWindow, kill_btn);
    gtk_widget_class_bind_template_child (widget_class, GooroomExampleWindow, active_tree);
    gtk_widget_class_bind_template_child (widget_class, GooroomExampleWindow, all_tree);
    gtk_widget_class_bind_template_child (widget_class, GooroomExampleWindow, my_tree);

    gtk_widget_class_bind_template_callback (widget_class, terminate_clicked);
    gtk_widget_class_bind_template_callback (widget_class, stop_clicked);
    gtk_widget_class_bind_template_callback (widget_class, continue_clicked);
    gtk_widget_class_bind_template_callback (widget_class, kill_clicked);

}
