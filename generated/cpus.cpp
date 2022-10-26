void output_cpu_stat_mode(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_cpu_t cpus[], size_t cpu_count, const std::function<double (perfstat_cpu_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	for(size_t i=0; i<cpu_count; i++) {
		response << name << "{cpu=\"" << cpus[i].name << "\",";
                response << static_labels << "} " << func(cpus[i]) << std::endl;
	}
}

void gather_cpus(std::ostream& response, const std::string& static_labels) {
    std::cout << "cpu metrics" << std::endl;
	int cpu_count = perfstat_cpu(NULL, NULL, sizeof(perfstat_cpu_t), 0);

	perfstat_cpu_t cpus[cpu_count];
	perfstat_id_t firstcpu;

	strcpy(firstcpu.name, FIRST_CPU);

	if(perfstat_cpu(&firstcpu, cpus, sizeof(perfstat_cpu_t), cpu_count) <= 0) {
		perror("Error calling perfstat_cpu");
		return;
	}

	output_cpu_stat_mode(response, static_labels, "aix_cpu_user", "counter", "raw number of clock ticks spent in user mode", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.user; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_sys", "counter", "raw number of clock ticks spent in system mode", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.sys; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_idle", "counter", "raw number of clock ticks spent idle", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.idle; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_wait", "counter", "raw number of clock ticks spent waiting for I/O", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.wait; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_pswitch", "counter", "number of context switches (changes of currently running process)", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.pswitch; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_syscall", "counter", "number of system calls executed", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.syscall; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_sysread", "counter", "number of read system calls executed", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.sysread; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_syswrite", "counter", "number of write system calls executed", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.syswrite; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_sysfork", "counter", "number of fork system call executed", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.sysfork; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_sysexec", "counter", "number of exec system call executed", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.sysexec; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_readch", "counter", "number of characters tranferred with read system call", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.readch; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_writech", "counter", "number of characters tranferred with write system call", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.writech; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_bread", "counter", "number of block reads", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.bread; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_bwrite", "counter", "number of block writes", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.bwrite; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_lread", "counter", "number of logical read requests", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.lread; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_lwrite", "counter", "number of logical write requests", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.lwrite; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_phread", "counter", "number of physical reads (reads on raw device)", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.phread; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_phwrite", "counter", "number of physical writes (writes on raw device)", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.phwrite; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_iget", "counter", "number of inode lookups", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.iget; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_namei", "counter", "number of vnode lookup from a path name", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.namei; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_dirblk", "counter", "number of 512-byte block reads by the directory search routine to locate an entry for a file", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.dirblk; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_msg", "counter", "number of IPC message operations", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.msg; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_sema", "counter", "number of IPC semaphore operations", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.sema; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_minfaults", "counter", "number of page faults with no I/O", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.minfaults; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_majfaults", "counter", "number of page faults with disk I/O", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.majfaults; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_puser", "counter", "raw number of physical processor tics in user mode", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.puser; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_psys", "counter", "raw number of physical processor tics in system mode", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.psys; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_pidle", "counter", "raw number of physical processor tics idle", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.pidle; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_pwait", "counter", "raw number of physical processor tics waiting for I/O", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.pwait; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_redisp_sd0", "counter", "number of thread redispatches within the scheduler affinity domain 0", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.redisp_sd0; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_redisp_sd1", "counter", "number of thread redispatches within the scheduler affinity domain 1", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.redisp_sd1; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_redisp_sd2", "counter", "number of thread redispatches within the scheduler affinity domain 2", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.redisp_sd2; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_redisp_sd3", "counter", "number of thread redispatches within the scheduler affinity domain 3", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.redisp_sd3; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_redisp_sd4", "counter", "number of thread redispatches within the scheduler affinity domain 4", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.redisp_sd4; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_redisp_sd5", "counter", "number of thread redispatches within the scheduler affinity domain 5", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.redisp_sd5; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_migration_push", "counter", "number of thread migrations from the local runque to another queue due to starvation load balancing", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.migration_push; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_migration_S3grq", "counter", "number of  thread migrations from the global runque to the local runque resulting in a move accross scheduling domain 3", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.migration_S3grq; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_migration_S3pul", "counter", "number of  thread migrations from another processor's runque resulting in a move accross scheduling domain 3", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.migration_S3pul; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_invol_cswitch", "counter", "number of  involuntary thread context switches", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.invol_cswitch; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_vol_cswitch", "counter", "number of  voluntary thread context switches", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.vol_cswitch; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_runque", "gauge", "number of  threads on the runque", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.runque; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_bound", "gauge", "number of  bound threads", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.bound; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_decrintrs", "counter", "number of decrementer tics interrupts", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.decrintrs; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_mpcrintrs", "counter", "number of mpc's received interrupts", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.mpcrintrs; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_mpcsintrs", "counter", "number of mpc's sent interrupts", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.mpcsintrs; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_devintrs", "counter", "number of device interrupts", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.devintrs; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_softintrs", "counter", "number of offlevel handlers called", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.softintrs; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_phantintrs", "counter", "number of phantom interrupts", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.phantintrs; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_idle_donated_purr", "counter", "number of idle cycles donated by a dedicated partition enabled for donation", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.idle_donated_purr; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_idle_donated_spurr", "counter", "number of idle spurr cycles donated by a dedicated partition enabled for donation", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.idle_donated_spurr; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_busy_donated_purr", "counter", "number of busy cycles donated by a dedicated partition enabled for donation", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.busy_donated_purr; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_busy_donated_spurr", "counter", "number of busy spurr cycles donated by a dedicated partition enabled for donation", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.busy_donated_spurr; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_idle_stolen_purr", "counter", "number of idle cycles stolen by the hypervisor from a dedicated partition", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.idle_stolen_purr; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_idle_stolen_spurr", "counter", "number of idle spurr cycles stolen by the hypervisor from a dedicated partition", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.idle_stolen_spurr; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_busy_stolen_purr", "counter", "number of busy cycles stolen by the hypervisor from a dedicated partition", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.busy_stolen_purr; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_busy_stolen_spurr", "counter", "number of busy spurr cycles stolen by the hypervisor from a dedicated partition", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.busy_stolen_spurr; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_hpi", "counter", "number of hypervisor page-ins", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.hpi; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_hpit", "counter", "Time spent in hypervisor page-ins (in nanoseconds)", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.hpit; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_puser_spurr", "counter", "number of spurr cycles spent in user mode", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.puser_spurr; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_psys_spurr", "counter", "number of spurr cycles spent in kernel mode", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.psys_spurr; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_pidle_spurr", "counter", "number of spurr cycles spent in idle mode", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.pidle_spurr; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_pwait_spurr", "counter", "number of spurr cycles spent in wait mode", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.pwait_spurr; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_spurrflag", "gauge", "set if running in spurr mode", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.spurrflag; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_localdispatch", "counter", "number of local thread dispatches on this logical CPU", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.localdispatch; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_neardispatch", "counter", "number of near thread dispatches on this logical CPU", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.neardispatch; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_fardispatch", "counter", "number of far thread dispatches on this logical CPU", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.fardispatch; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_cswitches", "counter", "Context switches", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.cswitches; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_tb_last", "counter", "timebase counter", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.tb_last; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_state", "gauge", "Show whether the CPU is offline or online", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.state; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_vtb_last", "gauge", "Last virtual timebase read", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.vtb_last; });
	output_cpu_stat_mode(response, static_labels, "aix_cpu_icount_last", "gauge", "Last instruction count read", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return cpu.icount_last; });
}
