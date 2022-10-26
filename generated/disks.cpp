void output_disk_stat_mode(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_disk_t disks[], size_t disk_count, const std::function<double (perfstat_disk_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	for(size_t i=0; i<disk_count; i++) {
		response << name << "{disk=\"" << disks[i].name << "\",";
		response << "vgname=\"" <<  disks[i].vgname  << "\",";
                response << static_labels << "} " << func(disks[i]) << std::endl;
	}
}

void gather_disks(std::ostream& response, const std::string& static_labels) {
    std::cout << "disks metrics" << std::endl;
	int disk_count = perfstat_disk(NULL, NULL, sizeof(perfstat_disk_t), 0);

	perfstat_disk_t disks[disk_count];
	perfstat_id_t firstdisk;

	strcpy(firstdisk.name, FIRST_DISK);

	if(perfstat_disk(&firstdisk, disks, sizeof(perfstat_disk_t), disk_count) <= 0) {
		perror("Error calling perfstat_disk");
		return;
	}

	output_disk_stat_mode(response, static_labels, "aix_disk_size", "gauge", "size of the disk (in MB)", disks, disk_count, [](perfstat_disk_t& disk) { return disk.size; });
	output_disk_stat_mode(response, static_labels, "aix_disk_free", "gauge", "free portion of the disk (in MB)", disks, disk_count, [](perfstat_disk_t& disk) { return disk.free; });
	output_disk_stat_mode(response, static_labels, "aix_disk_bsize", "gauge", "disk block size (in bytes)", disks, disk_count, [](perfstat_disk_t& disk) { return disk.bsize; });
	output_disk_stat_mode(response, static_labels, "aix_disk_xrate", "counter", "number of transfers from disk", disks, disk_count, [](perfstat_disk_t& disk) { return disk.xrate; });
	output_disk_stat_mode(response, static_labels, "aix_disk_xfers", "counter", "number of transfers to/from disk", disks, disk_count, [](perfstat_disk_t& disk) { return disk.xfers; });
	output_disk_stat_mode(response, static_labels, "aix_disk_wblks", "counter", "number of blocks written to disk", disks, disk_count, [](perfstat_disk_t& disk) { return disk.wblks; });
	output_disk_stat_mode(response, static_labels, "aix_disk_rblks", "counter", "number of blocks read from disk", disks, disk_count, [](perfstat_disk_t& disk) { return disk.rblks; });
	output_disk_stat_mode(response, static_labels, "aix_disk_qdepth", "gauge", "instantaneous 'service' queue depth (number of requests sent to disk and not completed yet)", disks, disk_count, [](perfstat_disk_t& disk) { return disk.qdepth; });
	output_disk_stat_mode(response, static_labels, "aix_disk_time", "counter", "amount of time disk is active", disks, disk_count, [](perfstat_disk_t& disk) { return disk.time; });
	output_disk_stat_mode(response, static_labels, "aix_disk_paths_count", "gauge", "number of paths to this disk", disks, disk_count, [](perfstat_disk_t& disk) { return disk.paths_count; });
	output_disk_stat_mode(response, static_labels, "aix_disk_q_full", "counter", "'service' queue full occurrence count (number of times the disk is not accepting any more request)", disks, disk_count, [](perfstat_disk_t& disk) { return disk.q_full; });
	output_disk_stat_mode(response, static_labels, "aix_disk_rserv", "counter", "read or receive service time", disks, disk_count, [](perfstat_disk_t& disk) { return disk.rserv; });
	output_disk_stat_mode(response, static_labels, "aix_disk_rtimeout", "counter", "number of read request timeouts", disks, disk_count, [](perfstat_disk_t& disk) { return disk.rtimeout; });
	output_disk_stat_mode(response, static_labels, "aix_disk_rfailed", "counter", "number of failed read requests", disks, disk_count, [](perfstat_disk_t& disk) { return disk.rfailed; });
	output_disk_stat_mode(response, static_labels, "aix_disk_min_rserv", "gauge", "min read or receive service time", disks, disk_count, [](perfstat_disk_t& disk) { return disk.min_rserv; });
	output_disk_stat_mode(response, static_labels, "aix_disk_max_rserv", "gauge", "max read or receive service time", disks, disk_count, [](perfstat_disk_t& disk) { return disk.max_rserv; });
	output_disk_stat_mode(response, static_labels, "aix_disk_wserv", "counter", "write or send service time", disks, disk_count, [](perfstat_disk_t& disk) { return disk.wserv; });
	output_disk_stat_mode(response, static_labels, "aix_disk_wtimeout", "counter", "number of write request timeouts", disks, disk_count, [](perfstat_disk_t& disk) { return disk.wtimeout; });
	output_disk_stat_mode(response, static_labels, "aix_disk_wfailed", "counter", "number of failed write requests", disks, disk_count, [](perfstat_disk_t& disk) { return disk.wfailed; });
	output_disk_stat_mode(response, static_labels, "aix_disk_min_wserv", "gauge", "min write or send service time", disks, disk_count, [](perfstat_disk_t& disk) { return disk.min_wserv; });
	output_disk_stat_mode(response, static_labels, "aix_disk_max_wserv", "gauge", "max write or send service time", disks, disk_count, [](perfstat_disk_t& disk) { return disk.max_wserv; });
	output_disk_stat_mode(response, static_labels, "aix_disk_wq_depth", "gauge", "instantaneous wait queue depth (number of requests waiting to be sent to disk)", disks, disk_count, [](perfstat_disk_t& disk) { return disk.wq_depth; });
	output_disk_stat_mode(response, static_labels, "aix_disk_wq_sampled", "counter", "accumulated sampled dk_wq_depth", disks, disk_count, [](perfstat_disk_t& disk) { return disk.wq_sampled; });
	output_disk_stat_mode(response, static_labels, "aix_disk_wq_time", "counter", "accumulated wait queueing time", disks, disk_count, [](perfstat_disk_t& disk) { return disk.wq_time; });
	output_disk_stat_mode(response, static_labels, "aix_disk_wq_min_time", "gauge", "min wait queueing time", disks, disk_count, [](perfstat_disk_t& disk) { return disk.wq_min_time; });
	output_disk_stat_mode(response, static_labels, "aix_disk_wq_max_time", "gauge", "max wait queueing time", disks, disk_count, [](perfstat_disk_t& disk) { return disk.wq_max_time; });
	output_disk_stat_mode(response, static_labels, "aix_disk_q_sampled", "counter", "accumulated sampled dk_q_depth", disks, disk_count, [](perfstat_disk_t& disk) { return disk.q_sampled; });
}
