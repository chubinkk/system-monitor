/* subin-common.c
 *
 * Copyright 2020 subin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <pwd.h>

#include <glibtop/cpu.h>
#include <glibtop/proclist.h>
#include <glibtop/procstate.h>
#include <glibtop/procuid.h>
#include <glibtop/procmem.h>

#include "model.h"
#include "subin-common.h"
#include "subin-view-model-listeners.h"

static COMMON_INFO *info = NULL;

static gchar *g_page_name = "active process"; 
//static int g_flag = 0; 

void 
set_page (gchar *page_name)
//set_flag (int flag)
{
    g_page_name = page_name;    
}

static gboolean
update_data (gpointer data)
{
    COMMON_INFO   *infos;
    EventHandlers *handlers = NULL;

    struct passwd*      pwd;

    pid_t*              pid_list;
    glibtop_proclist    proclist;
    glibtop_proc_state  proc_state;
    glibtop_proc_uid    proc_uid;
    glibtop_proc_mem    proc_mem;

    gint i;
    gint whose = 0;
    gint arg = 0;

	infos = data;

#if 1
    if (strcmp (g_page_name, "active process") == 0)
//    if (g_flag == 0)
    {
        whose = GLIBTOP_KERN_PROC_ALL | GLIBTOP_EXCLUDE_IDLE;
    }
    else if (strcmp (g_page_name , "all process") == 0)
//    else if (g_flag == 1)
    {
        whose = GLIBTOP_KERN_PROC_ALL;
    }
    else if (strcmp (g_page_name , "my process") == 0)
//    else if (g_flag  == 2)
    {
        whose = GLIBTOP_KERN_PROC_UID;
        arg = getuid();
    }

    pid_list = glibtop_get_proclist (&proclist, whose, arg);
#endif

    for (i = infos->sysinfos->len -1; 0 <= i; i--)
	{
	    ProcInfo *info = g_ptr_array_index (infos->sysinfos, i);
	    g_ptr_array_remove (infos->sysinfos, info);
		if (info)
		    free (info);
	}

    for (i = 0; i < proclist.number; i++)
    {
        ProcInfo *info;
     
        glibtop_get_proc_state (&proc_state, pid_list[i]);
        glibtop_get_proc_uid (&proc_uid, pid_list[i]);
        glibtop_get_proc_mem (&proc_mem, pid_list[i]);

		info = malloc (sizeof(ProcInfo));
        info->process_name = g_strdup (proc_state.cmd);
        info->pid = pid_list[i];
        info->uid = proc_uid.uid;

        pwd = getpwuid (info->uid);          
        if (pwd && pwd->pw_name)
        {
            info->user = g_strdup (pwd->pw_name);
        }
        else
        {
            gchar *username;
            username = g_strdup_printf ("%d", info->uid);
            info->user = username;
        }

        info->cpu = proc_state.has_cpu;
        info->memory = proc_mem.rss;
        info->priority = proc_uid.priority;
        info->nice = proc_uid.nice;
        info->virtual_memory = proc_mem.vsize;
        info->resident_memory = proc_mem.resident;
        info->shared_memory = proc_mem.share;

        /*process state*/
        if(proc_state.state == GLIBTOP_PROCESS_RUNNING )
            info->status = g_strdup_printf("Running");
        else if(proc_state.state == GLIBTOP_PROCESS_INTERRUPTIBLE)
            info->status = g_strdup_printf("Interruptible");
        else if(proc_state.state == GLIBTOP_PROCESS_UNINTERRUPTIBLE)
            info->status = g_strdup_printf("Uninterruptible");
        else if(proc_state.state == GLIBTOP_PROCESS_STOPPED)
            info->status = g_strdup_printf("Stopped");
        else if(proc_state.state == GLIBTOP_PROCESS_ZOMBIE)
            info->status = g_strdup_printf("Zombie");
        else if(proc_state.state == GLIBTOP_PROCESS_SWAPPING)
            info->status = g_strdup_printf("Swapping");
        else if(proc_state.state == GLIBTOP_PROCESS_DEAD)
            info->status = g_strdup_printf("Dead");
        else
            info->status = g_strdup_printf("Sleeping");

	    g_ptr_array_add (infos->sysinfos, info);
    }
    
	handlers = subin_view_model_listeners_get (EVENT_UPDATE);
    for (; handlers != NULL; handlers = handlers->next)
    {
        handlers->cb(info->sysinfos, handlers->data);
    }
	
	return G_SOURCE_CONTINUE;
}

void
end_job ()
{
    if (info != NULL)
    {
        g_source_remove (info->id);
		g_ptr_array_free (info->sysinfos, TRUE);
        free (info);
        info = NULL;
    }
}

void
start_job ()
{
    if (info == NULL)
    {
        info = malloc (sizeof (COMMON_INFO));
        //info->sysinfos = g_ptr_array_new_with_free_func ((GDestroyNotify)free);
        info->sysinfos = g_ptr_array_new ();
    }

    info->id = g_timeout_add (1000, update_data, info);
}
