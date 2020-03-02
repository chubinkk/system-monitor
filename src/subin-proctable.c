/* subin-proctable.c
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
#include <glibtop/procmem.h>

#include "subin-config.h"
#include "subin-window.h"
#include "treeview.h"

void
get_process_info(ProcInfo info[])
{
    pid_t*              pid_list; 
//    g_autofree gchar   *pid;
    glibtop_cpu         cpu;
    glibtop_proclist    proclist;
    glibtop_proc_state  procstate;
    glibtop_proc_uid    procuid;
    glibtop_proc_mem    proc_mem;

    
    gint i;
    gint arg = 0;
    GError *error = NULL;

    pid_list = glibtop_get_proclist(&proclist, GLIBTOP_KERN_PROC_ALL, arg);

    for (i = 0; i < proclist.number; i++)
    {
        glibtop_get_proc_state(&procstate,pid_list[i]);
        glibtop_get_proc_mem(&proc_mem,pid_list[i]);

        info[i].pid = pid_list[i];
        info[i].process_name = g_strdup_printf("%s",procstate.cmd);
        info[i].cpu = procstate.has_cpu;
        info[i].memory = proc_mem.rss;
        
        g_print ("pid: %d\n process name: %s\n cpu: %d\n mem: %ld\n",info[i].pid, info[i].process_name, info[i].cpu, info[i].memory);
    }

/*-------------------------------------------------------------------------------------------*/
//    pid_list = glibtop_get_proclist (&proclist, GLIBTOP_KERN_PROC_ALL, arg);
//    
//    for (i = 0; i < proclist.number; i++)
//        g_print ("pid[%d]: %d\n",i, pid_list[i]);
// 
//    glibtop_get_cpu (&cpu);
//    
//    glibtop_get_proc_state (&procstate, pid_list[i]);
/*-------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*/
//    g_strdup_printf ("Total %ld , User:%ld\n", cpu.total, cpu.user);
//    g_strdup_printf ("Proclist number: %ld , Proclist total: %ld , Proclist size: %ld\n", proclist.number, proclist.total, proclist.size);
//    g_strdup_printf ("Proc_state: %s\n", procstate.cmd);
//    glibtop_get_proc_uid (pid, pid_list);
/*-------------------------------------------------------------------------------------------*/

}

