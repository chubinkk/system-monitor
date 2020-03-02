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

#include <glibtop/cpu.h>
#include <glibtop/proclist.h>
#include <glibtop/procstate.h>
#include <glibtop/procuid.h>

#include "subin-config.h"
#include "subin-window.h"
#include "treeview.c"
#include "subin-proctable.c"

struct _SubinWindow
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

G_DEFINE_TYPE (SubinWindow, subin_window, GTK_TYPE_APPLICATION_WINDOW)

static void
search_btn_clicked (GtkButton *button, SubinWindow *win)
{
    g_print ("= = = = = = => terminate button clicked \n");
}

static void
terminate_btn_clicked (GtkButton *button, SubinWindow *win)
{
    g_print ("= = = = = = => terminate button clicked \n");
}

static void
stop_btn_clicked (GtkButton *button, SubinWindow *win)
{
    g_print ("= = = = = = => stop button clicked \n");
}

static void
continue_btn_clicked (GtkButton *button, SubinWindow *win)
{
    g_print ("= = = = = = => continue button clicked \n");
}

static void
kill_btn_clicked (GtkButton *button, SubinWindow *win)
{
    g_print ("= = = = = = => kill button clicked \n");
}

static void
subin_window_constructed (GObject *obj) 
{
    G_OBJECT_CLASS(subin_window_parent_class) -> constructed(obj);
    SubinWindow *win = SUBIN_WINDOW(obj);    
    
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

//    get_process_info();
}

static void
subin_window_init (SubinWindow *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));
    gtk_widget_show_all (GTK_WIDGET (self));
}

static void
subin_window_class_init (SubinWindowClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
#if 0
    GObjectClass *oclass = G_OBJECT_CLASS (klass);
    oclass->dispose = subin_window_dispose;
    oclass->finalize = subin_window_finalize;
    oclass->constructed = subin_window_constructed;
    
	widget_class->destroy = subin_window_destroy;
    widget_class->show = subin_window_show;
    widget_class->realize = subin_window_realize;
    widget_class->key_press_event = subin_window_key_press_event;
    widget_class->delete_event = subin_window_delete_event;
    widget_class->grab_focus = subin_window_grab_focus;
#endif

    GObjectClass *oclass = G_OBJECT_CLASS (klass);
    oclass->constructed = subin_window_constructed;
    
    gtk_widget_class_set_template_from_resource (widget_class, "/kr/gooroom/Gooroom-Example/subin-window.ui");
    gtk_widget_class_bind_template_child (widget_class, SubinWindow, header_bar);
    gtk_widget_class_bind_template_child (widget_class, SubinWindow, search_btn);
    gtk_widget_class_bind_template_child (widget_class, SubinWindow, terminate_btn);
    gtk_widget_class_bind_template_child (widget_class, SubinWindow, stop_btn);
    gtk_widget_class_bind_template_child (widget_class, SubinWindow, kill_btn);
    gtk_widget_class_bind_template_child (widget_class, SubinWindow, active_tree);
    gtk_widget_class_bind_template_child (widget_class, SubinWindow, all_tree);
    gtk_widget_class_bind_template_child (widget_class, SubinWindow, my_tree);

    gtk_widget_class_bind_template_callback (widget_class, search_btn_clicked);
    gtk_widget_class_bind_template_callback (widget_class, terminate_btn_clicked);
    gtk_widget_class_bind_template_callback (widget_class, stop_btn_clicked);
    gtk_widget_class_bind_template_callback (widget_class, continue_btn_clicked);
    gtk_widget_class_bind_template_callback (widget_class, kill_btn_clicked);

}
