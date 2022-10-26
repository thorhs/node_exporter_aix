void output_memory_stat(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_memory_total_t memory, const std::function<double (perfstat_memory_total_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	response << name << "{" << static_labels << "} " << func(memory) << std::endl;
}

void gather_memory(std::ostream& response, const std::string& static_labels) {
    std::cout << "memory metrics" << std::endl;

    perfstat_memory_total_t memory;

	if(perfstat_memory_total(NULL, &memory, sizeof(perfstat_memory_total_t), 1) <= 0) {
		perror("Error calling perfstat_memory_total");
		return;
	}

	output_memory_stat(response, static_labels, "aix_memory_virt_total", "gauge", "total virtual memory (in 4KB pages)", memory, [](perfstat_memory_total_t& memory) { return memory.virt_total; });
	output_memory_stat(response, static_labels, "aix_memory_real_total", "gauge", "total real memory (in 4KB pages)", memory, [](perfstat_memory_total_t& memory) { return memory.real_total; });
	output_memory_stat(response, static_labels, "aix_memory_real_free", "gauge", "free real memory (in 4KB pages)", memory, [](perfstat_memory_total_t& memory) { return memory.real_free; });
	output_memory_stat(response, static_labels, "aix_memory_real_pinned", "gauge", "real memory which is pinned (in 4KB pages)", memory, [](perfstat_memory_total_t& memory) { return memory.real_pinned; });
	output_memory_stat(response, static_labels, "aix_memory_real_inuse", "gauge", "real memory which is in use (in 4KB pages)", memory, [](perfstat_memory_total_t& memory) { return memory.real_inuse; });
	output_memory_stat(response, static_labels, "aix_memory_pgbad", "gauge", "number of bad pages", memory, [](perfstat_memory_total_t& memory) { return memory.pgbad; });
	output_memory_stat(response, static_labels, "aix_memory_pgexct", "gauge", "number of page faults", memory, [](perfstat_memory_total_t& memory) { return memory.pgexct; });
	output_memory_stat(response, static_labels, "aix_memory_pgins", "gauge", "number of pages paged in", memory, [](perfstat_memory_total_t& memory) { return memory.pgins; });
	output_memory_stat(response, static_labels, "aix_memory_pgouts", "gauge", "number of pages paged out", memory, [](perfstat_memory_total_t& memory) { return memory.pgouts; });
	output_memory_stat(response, static_labels, "aix_memory_pgspins", "gauge", "number of page ins from paging space", memory, [](perfstat_memory_total_t& memory) { return memory.pgspins; });
	output_memory_stat(response, static_labels, "aix_memory_pgspouts", "gauge", "number of page outs from paging space", memory, [](perfstat_memory_total_t& memory) { return memory.pgspouts; });
	output_memory_stat(response, static_labels, "aix_memory_scans", "gauge", "number of page scans by clock", memory, [](perfstat_memory_total_t& memory) { return memory.scans; });
	output_memory_stat(response, static_labels, "aix_memory_cycles", "gauge", "number of page replacement cycles", memory, [](perfstat_memory_total_t& memory) { return memory.cycles; });
	output_memory_stat(response, static_labels, "aix_memory_pgsteals", "gauge", "number of page steals", memory, [](perfstat_memory_total_t& memory) { return memory.pgsteals; });
	output_memory_stat(response, static_labels, "aix_memory_numperm", "gauge", "number of frames used for files (in 4KB pages)", memory, [](perfstat_memory_total_t& memory) { return memory.numperm; });
	output_memory_stat(response, static_labels, "aix_memory_pgsp_total", "gauge", "total paging space (in 4KB pages)", memory, [](perfstat_memory_total_t& memory) { return memory.pgsp_total; });
	output_memory_stat(response, static_labels, "aix_memory_pgsp_free", "gauge", "free paging space (in 4KB pages)", memory, [](perfstat_memory_total_t& memory) { return memory.pgsp_free; });
	output_memory_stat(response, static_labels, "aix_memory_pgsp_rsvd", "gauge", "reserved paging space (in 4KB pages)", memory, [](perfstat_memory_total_t& memory) { return memory.pgsp_rsvd; });
	output_memory_stat(response, static_labels, "aix_memory_real_system", "gauge", "real memory used by system segments (in 4KB pages). This is the sum of all the used pages in segment marked for system usage. Since segment classifications are not always guaranteed to be accurate, this number is only an approximation.", memory, [](perfstat_memory_total_t& memory) { return memory.real_system; });
	output_memory_stat(response, static_labels, "aix_memory_real_user", "gauge", "real memory used by non-system segments (in 4KB pages). This is the sum of all pages used in segments not marked for system usage. Since segment classifications are not always guaranteed to be accurate, this number is only an approximation.", memory, [](perfstat_memory_total_t& memory) { return memory.real_user; });
	output_memory_stat(response, static_labels, "aix_memory_real_process", "gauge", "real memory used by process segments (in 4KB pages). This is real_total-real_free-numperm-real_system. Since real_system is an approximation, this number is too.", memory, [](perfstat_memory_total_t& memory) { return memory.real_process; });
	output_memory_stat(response, static_labels, "aix_memory_virt_active", "gauge", "Active virtual pages. Virtual pages are considered active if they have been accessed", memory, [](perfstat_memory_total_t& memory) { return memory.virt_active; });
	output_memory_stat(response, static_labels, "aix_memory_iome", "gauge", "I/O memory entitlement of the partition in bytes", memory, [](perfstat_memory_total_t& memory) { return memory.iome; });
	output_memory_stat(response, static_labels, "aix_memory_iomu", "gauge", "I/O memory entitlement of the partition in use in bytes", memory, [](perfstat_memory_total_t& memory) { return memory.iomu; });
	output_memory_stat(response, static_labels, "aix_memory_iohwm", "gauge", "High water mark of I/O memory entitlement used in bytes", memory, [](perfstat_memory_total_t& memory) { return memory.iohwm; });
	output_memory_stat(response, static_labels, "aix_memory_pmem", "gauge", "Amount of physical mmeory currently backing partition's logical memory in bytes", memory, [](perfstat_memory_total_t& memory) { return memory.pmem; });
	output_memory_stat(response, static_labels, "aix_memory_comprsd_total", "gauge", "Total numbers of pages in compressed pool (in 4KB pages)", memory, [](perfstat_memory_total_t& memory) { return memory.comprsd_total; });
	output_memory_stat(response, static_labels, "aix_memory_comprsd_wseg_pgs", "gauge", "Number of compressed working storage pages", memory, [](perfstat_memory_total_t& memory) { return memory.comprsd_wseg_pgs; });
	output_memory_stat(response, static_labels, "aix_memory_cpgins", "counter", "number of page ins to compressed pool", memory, [](perfstat_memory_total_t& memory) { return memory.cpgins; });
	output_memory_stat(response, static_labels, "aix_memory_cpgouts", "counter", "number of page outs from compressed pool", memory, [](perfstat_memory_total_t& memory) { return memory.cpgouts; });
	output_memory_stat(response, static_labels, "aix_memory_true_size", "gauge", "True Memory Size in 4KB pages", memory, [](perfstat_memory_total_t& memory) { return memory.true_size; });
	output_memory_stat(response, static_labels, "aix_memory_expanded_memory", "gauge", "Expanded Memory Size in 4KB pages", memory, [](perfstat_memory_total_t& memory) { return memory.expanded_memory; });
	output_memory_stat(response, static_labels, "aix_memory_comprsd_wseg_size", "gauge", "Total size of the compressed working storage pages in the pool", memory, [](perfstat_memory_total_t& memory) { return memory.comprsd_wseg_size; });
	output_memory_stat(response, static_labels, "aix_memory_target_cpool_size", "gauge", "Target Compressed Pool Size in bytes", memory, [](perfstat_memory_total_t& memory) { return memory.target_cpool_size; });
	output_memory_stat(response, static_labels, "aix_memory_max_cpool_size", "gauge", "Max Size of Compressed Pool in bytes", memory, [](perfstat_memory_total_t& memory) { return memory.max_cpool_size; });
	output_memory_stat(response, static_labels, "aix_memory_min_ucpool_size", "gauge", "Min Size of Uncompressed Pool in bytes", memory, [](perfstat_memory_total_t& memory) { return memory.min_ucpool_size; });
	output_memory_stat(response, static_labels, "aix_memory_cpool_size", "gauge", "Compressed Pool size in bytes", memory, [](perfstat_memory_total_t& memory) { return memory.cpool_size; });
	output_memory_stat(response, static_labels, "aix_memory_ucpool_size", "gauge", "Uncompressed Pool size in bytes", memory, [](perfstat_memory_total_t& memory) { return memory.ucpool_size; });
	output_memory_stat(response, static_labels, "aix_memory_cpool_inuse", "gauge", "Compressed Pool Used in bytes", memory, [](perfstat_memory_total_t& memory) { return memory.cpool_inuse; });
	output_memory_stat(response, static_labels, "aix_memory_ucpool_inuse", "gauge", "Uncompressed Pool Used in bytes", memory, [](perfstat_memory_total_t& memory) { return memory.ucpool_inuse; });
	output_memory_stat(response, static_labels, "aix_memory_real_avail", "gauge", "number of pages (in 4KB pages) of memory available without paging out working segments", memory, [](perfstat_memory_total_t& memory) { return memory.real_avail; });
	output_memory_stat(response, static_labels, "aix_memory_bytes_coalesced", "gauge", "The number of bytes of the calling partition.s logical real memory  coalesced because they contained duplicated data", memory, [](perfstat_memory_total_t& memory) { return memory.bytes_coalesced; });
	output_memory_stat(response, static_labels, "aix_memory_bytes_coalesced_mempool", "gauge", "If the calling partition is authorized to see pool wide statistics then the number of bytes of logical real memory coalesced because they contained duplicated data in the calling partition.s memory pool else set to zero.", memory, [](perfstat_memory_total_t& memory) { return memory.bytes_coalesced_mempool; });
}
