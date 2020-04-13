/* subin-window.c
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
#include "subin-common.h"
#include "subin-tree-view-model.h"
#include "subin-view-model-listeners.h"

struct _SubinWindow
{
    GtkApplicationWindow  parent_instance;
};

typedef struct
{
    /* Template widgets */
    GtkStackSwitcher    *stack_switcher;
    GtkStack            *stack;
    GtkSearchBar        *active_searchbar;
    GtkButton           *search_btn;
    GtkButton           *terminate_btn;       
    GtkButton           *stop_btn;
    GtkButton           *continue_btn;
    GtkButton           *kill_btn;

    GtkTreeView         *active_tree;
    GtkTreeView         *all_tree;
    GtkTreeView         *my_tree;
    GtkScrolledWindow   *active_scrolled;

    SubinTreeViewModel  *tree_view_model;
}SubinWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (SubinWindow, subin_window, GTK_TYPE_APPLICATION_WINDOW)

static void 
get_stack (GtkWidget *widget,
           GdkEvent *event,
           SubinWindow *win)
{
    const gchar *child_name;

    SubinWindowPrivate *priv = subin_window_get_instance_private (win);

    GtkStackSwitcher *stack_switcher = GTK_STACK_SWITCHER (priv->stack_switcher);
    GtkStack *stack = GTK_STACK (priv->stack);
    gtk_stack_switcher_set_stack (stack_switcher,stack);
    
    child_name = gtk_stack_get_visible_child_name (stack);
    g_print ("%s\n", child_name);
    subin_tree_view_model_clear (priv->tree_view_model);

    set_page (child_name);
}

static void
on_terminate_btn_clicked (GtkButton *button, SubinWindow *win)
{
    pid_t             pid;
    GtkTreeSelection *selection;
    GtkTreeModel     *model;
    GtkTreeIter       iter;
    gchar            *stack;

    GtkWidget        *confirm_dialog;
    GtkWidget        *error_dialog;
    gchar            *process_name;
    int               ret;
    
    SubinWindowPrivate *priv = subin_window_get_instance_private (win);

    stack = gtk_stack_get_visible_child_name(priv->stack);
 
    if (strcmp (stack, "active process") == 0)
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->active_tree));
    else if (strcmp (stack , "all process") == 0)
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->all_tree));
    else if (strcmp (stack , "my process") == 0)
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->my_tree));
   
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
    
    if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
        gtk_tree_model_get (model, &iter, PID_COLUMN, &pid, -1);
        gtk_tree_model_get (model, &iter, PROCESS_NAME_COLUMN, &process_name, -1);
        confirm_dialog = gtk_message_dialog_new (NULL,
                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                                                 GTK_MESSAGE_INFO,
                                                 GTK_BUTTONS_OK_CANCEL,
                                                 "정말 %s (PID: %d)을 Terminate하시겠습니까?",
                                                 process_name,
                                                 pid);
        ret = gtk_dialog_run (confirm_dialog);
        switch (ret)
        {
            case GTK_RESPONSE_OK:
                kill (pid, SIGTERM);
                break;
            case GTK_RESPONSE_CANCEL:
            default:
                break;
        }
        gtk_widget_destroy (confirm_dialog);
        if (errno == EPERM)
        {
            error_dialog = gtk_message_dialog_new (NULL,
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_CLOSE,
                                                   "Error message: %s",
                                                   g_strerror (errno)
                                                   );
            gtk_dialog_run (GTK_DIALOG (error_dialog));
            gtk_widget_destroy (error_dialog);
            g_free (process_name);
        }
    }
}

static void
on_stop_btn_clicked (GtkButton *button, SubinWindow *win)
{
    pid_t             pid;
    GtkTreeSelection *selection;
    GtkTreeModel     *model;
    GtkTreeIter       iter;
    gchar            *stack;

    SubinWindowPrivate *priv = subin_window_get_instance_private (win);

    stack = gtk_stack_get_visible_child_name(priv->stack);

    if (strcmp (stack, "active process") == 0)
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->active_tree));
    else if (strcmp (stack , "all process") == 0)
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->all_tree));
    else if (strcmp (stack , "my process") == 0)
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->my_tree));

    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
        gtk_tree_model_get (model, &iter, PID_COLUMN, &pid, -1);
        kill (pid, SIGSTOP);
    }

}

static void
on_continue_btn_clicked (GtkButton *button, SubinWindow *win)
{
    pid_t             pid;
    GtkTreeSelection *selection;
    GtkTreeModel     *model;
    GtkTreeIter       iter;
    gchar            *stack;

    SubinWindowPrivate *priv = subin_window_get_instance_private (win);

    stack = gtk_stack_get_visible_child_name(priv->stack);
 
    if (strcmp (stack, "active process") == 0)
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->active_tree));
    else if (strcmp (stack , "all process") == 0)
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->all_tree));
    else if (strcmp (stack , "my process") == 0)
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->my_tree));

    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
        gtk_tree_model_get (model, &iter, PID_COLUMN, &pid, -1);
        kill (pid, SIGCONT);
    }
}

void
on_kill_btn_clicked (GtkButton *button, SubinWindow *win)
{
    pid_t             pid;
    GtkTreeSelection *selection;
    GtkTreeModel     *model;
    GtkTreeIter       iter;
    gchar            *stack;

    GtkWidget        *confirm_dialog;
    GtkWidget        *error_dialog;
    gchar            *process_name;
    int               ret;

    SubinWindowPrivate *priv = subin_window_get_instance_private (win);

    stack = gtk_stack_get_visible_child_name(priv->stack);
 
    if (strcmp (stack, "active process") == 0)
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->active_tree));
    else if (strcmp (stack , "all process") == 0)
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->all_tree));
    else if (strcmp (stack , "my process") == 0)
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->my_tree));

    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
        gtk_tree_model_get (model, &iter, PID_COLUMN, &pid, -1);
        gtk_tree_model_get (model, &iter, PROCESS_NAME_COLUMN, &process_name, -1);
        confirm_dialog = gtk_message_dialog_new (NULL,
                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                                                 GTK_MESSAGE_INFO,
                                                 GTK_BUTTONS_OK_CANCEL,
                                                 "정말 %s (PID: %d)을 Kill하시겠습니까?",
                                                 process_name,
                                                 pid);
        ret = gtk_dialog_run (confirm_dialog);
        switch (ret)
        {
            case GTK_RESPONSE_OK:
                kill (pid, SIGKILL);
                break;
            case GTK_RESPONSE_CANCEL:
            default:
                break;
        }
        gtk_widget_destroy (confirm_dialog);
        if (errno == EPERM)
        {
            error_dialog = gtk_message_dialog_new (NULL,
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_CLOSE,
                                                   "Error message: %s",
                                                   g_strerror (errno)
                                                   );
            gtk_dialog_run (GTK_DIALOG (error_dialog));
            gtk_widget_destroy (error_dialog);
            g_free (process_name);
        }
    }

#if 0
    if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
        gtk_tree_model_get (model, &iter, PID_COLUMN, &pid, -1);
        kill (pid, SIGKILL);
    }
#endif

}

static gboolean
on_key_press_search_event (GtkWidget *widget,
                           GdkEvent  *event,
                           gpointer   user_data)
{
    GtkSearchBar *active_searchbar = GTK_SEARCH_BAR (user_data);
    GtkSearchEntry *searchentry = gtk_search_entry_new ();
    
    gtk_search_bar_connect_entry (active_searchbar, GTK_ENTRY(searchentry));
    return gtk_search_bar_handle_event (active_searchbar, event);
}


static gboolean
subin_window_start_job (gpointer data)
{
    start_job ();
	
	return G_SOURCE_REMOVE;
}

static void
subin_window_constructed (GObject *obj) 
{
    G_OBJECT_CLASS(subin_window_parent_class) -> constructed(obj);
}

static void
subin_window_dispose (GObject *self)
{
    SubinWindow *win;
    SubinWindowPrivate *priv;

    g_return_if_fail (self != NULL);

    win = SUBIN_WINDOW (self);
    priv = subin_window_get_instance_private (win);

    if (priv->tree_view_model)
    {
        g_object_unref (priv->tree_view_model);
        priv->tree_view_model = NULL;
    }
    
    G_OBJECT_CLASS (subin_window_parent_class)->dispose (self);

    subin_view_model_listeners_destroy();
    end_job();
}

static void
subin_window_finalize (GObject *self)
{
    G_OBJECT_CLASS (subin_window_parent_class)->finalize (self);
}


static void
subin_window_init (SubinWindow *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));

    SubinWindow *win;
    win = SUBIN_WINDOW (self);
    SubinWindowPrivate *priv = subin_window_get_instance_private (win);

    //Listener Init
    subin_view_model_listeners_init ();

    g_signal_connect (GTK_WIDGET (priv->stack_switcher),
                      "button-release-event",
                      G_CALLBACK (get_stack),
                      self);

    g_signal_connect (win,
                     "key-press-event",
                      G_CALLBACK (on_key_press_search_event),
                      priv->active_searchbar);
    
    //Tree model set
    priv->tree_view_model = subin_tree_view_model_new ();

    subin_tree_view_model_add_columns  (priv->active_tree);
    gtk_tree_view_set_model (priv->active_tree, GTK_TREE_MODEL (priv->tree_view_model));
    
    subin_tree_view_model_add_columns  (priv->all_tree);
    gtk_tree_view_set_model (priv->all_tree, GTK_TREE_MODEL (priv->tree_view_model));
    
    subin_tree_view_model_add_columns  (priv->my_tree);
    gtk_tree_view_set_model (priv->my_tree, GTK_TREE_MODEL (priv->tree_view_model));

    g_timeout_add (100, subin_window_start_job, NULL);
    
}


static void
subin_window_class_init (SubinWindowClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
#if 0
    GObjectClass *oclass = G_OBJECT_CLASS (klass);
    oclass->constructed = subin_window_constructed;
    
	widget_class->destroy = subin_window_destroy;
    widget_class->show = subin_window_show;
    widget_class->realize = subin_window_realize;
    widget_class->key_press_event = subin_window_key_press_event;
    widget_class->delete_event = subin_window_delete_event;
    widget_class->grab_focus = subin_window_grab_focus;
#endif

    GObjectClass *oclass = G_OBJECT_CLASS (klass);
    oclass->dispose = subin_window_dispose;
    oclass->finalize = subin_window_finalize;
    oclass->constructed = subin_window_constructed;
    
    gtk_widget_class_set_template_from_resource (widget_class, "/kr/gooroom/Subin/subin-window.ui");
    gtk_widget_class_bind_template_child_private (widget_class, SubinWindow, stack_switcher);
    gtk_widget_class_bind_template_child_private (widget_class, SubinWindow, stack);
    gtk_widget_class_bind_template_child_private (widget_class, SubinWindow, active_searchbar);
    gtk_widget_class_bind_template_child_private (widget_class, SubinWindow, terminate_btn);
    gtk_widget_class_bind_template_child_private (widget_class, SubinWindow, stop_btn);
    gtk_widget_class_bind_template_child_private (widget_class, SubinWindow, kill_btn);
    gtk_widget_class_bind_template_child_private (widget_class, SubinWindow, active_tree);
    gtk_widget_class_bind_template_child_private (widget_class, SubinWindow, all_tree);
    gtk_widget_class_bind_template_child_private (widget_class, SubinWindow, my_tree);
    gtk_widget_class_bind_template_child_private (widget_class, SubinWindow, active_scrolled);

    gtk_widget_class_bind_template_callback (widget_class, on_terminate_btn_clicked);
    gtk_widget_class_bind_template_callback (widget_class, on_stop_btn_clicked);
    gtk_widget_class_bind_template_callback (widget_class, on_continue_btn_clicked);
    gtk_widget_class_bind_template_callback (widget_class, on_kill_btn_clicked);
}
