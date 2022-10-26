void output_diskpath_stat_mode(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_diskpath_t diskpaths[], size_t diskpath_count, const std::function<double (perfstat_diskpath_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	for(size_t i=0; i<diskpath_count; i++) {
		response << name << "{diskpath=\"" << diskpaths[i].name << "\",";
                response << static_labels << "} " << func(diskpaths[i]) << std::endl;
	}
}

void gather_diskpaths(std::ostream& response, const std::string& static_labels) {
    std::cout << "disk paths metrics" << std::endl;
	int diskpath_count = perfstat_diskpath(NULL, NULL, sizeof(perfstat_diskpath_t), 0);

	perfstat_diskpath_t diskpaths[diskpath_count];
	perfstat_id_t firstdiskpath;

	strcpy(firstdiskpath.name, FIRST_DISKPATH);

	if(perfstat_diskpath(&firstdiskpath, diskpaths, sizeof(perfstat_diskpath_t), diskpath_count) <= 0) {
		perror("Error calling perfstat_diskpath");
		return;
	}

	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_xrate", "counter", "__rxfers: number of reads via the path", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.xrate; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_xfers", "counter", "number of transfers via the path", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.xfers; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_rblks", "counter", "512 bytes blocks written via the path", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.rblks; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_wblks", "counter", "512 bytes blocks read via the path", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.wblks; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_time", "counter", "amount of time disks are active", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.time; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_q_full", "counter", "'service' queue full occurrence count (number of times the disk is not accepting any more request)", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.q_full; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_rserv", "counter", "read or receive service time", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.rserv; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_rtimeout", "counter", "number of read request timeouts", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.rtimeout; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_rfailed", "counter", "number of failed read requests", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.rfailed; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_min_rserv", "counter", "min read or receive service time", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.min_rserv; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_max_rserv", "counter", "max read or receive service time", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.max_rserv; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_wserv", "counter", "write or send service time", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.wserv; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_wtimeout", "counter", "number of write request timeouts", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.wtimeout; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_wfailed", "counter", "number of failed write requests", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.wfailed; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_min_wserv", "counter", "min write or send service time", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.min_wserv; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_max_wserv", "counter", "max write or send service time", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.max_wserv; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_wq_depth", "counter", "instantaneous wait queue depth (number of requests waiting to be sent to disk)", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.wq_depth; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_wq_sampled", "counter", "accumulated sampled dk_wq_depth", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.wq_sampled; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_wq_time", "counter", "accumulated wait queueing time", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.wq_time; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_wq_min_time", "counter", "min wait queueing time", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.wq_min_time; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_wq_max_time", "counter", "max wait queueing time", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.wq_max_time; });
	output_diskpath_stat_mode(response, static_labels, "aix_diskpath_q_sampled", "counter", "accumulated sampled dk_q_depth", diskpaths, diskpath_count, [](perfstat_diskpath_t& diskpath) { return diskpath.q_sampled; });
}
