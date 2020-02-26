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

