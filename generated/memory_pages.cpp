void output_memory_page_stat_mode(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_memory_page_t memory_pages[], size_t memory_page_count, const std::function<double (perfstat_memory_page_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	for(size_t i=0; i<memory_page_count; i++) {
		response << name << "{memory_page=\"" << memory_pages[i].psize << "\",";
                response << static_labels << "} " << func(memory_pages[i]) << std::endl;
	}
}

void gather_memory_pages(std::ostream& response, const std::string& static_labels) {
    std::cout << "memory pages metrics" << std::endl;

    int memory_page_count = perfstat_memory_page(NULL, NULL, sizeof(perfstat_memory_page_t), 0);

	perfstat_memory_page_t memory_pages[memory_page_count];
	perfstat_psize_t firstmemory_page;

	firstmemory_page.psize=0;

	if(perfstat_memory_page(&firstmemory_page, memory_pages, sizeof(perfstat_memory_page_t), memory_page_count) <= 0) {
		perror("Error calling perfstat_memory_page");
		return;
	}

	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_real_total", "gauge", "number of real memory frames of this page size", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.real_total; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_real_free", "gauge", "number of pages on free list", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.real_free; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_real_pinned", "gauge", "number of pages pinned", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.real_pinned; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_real_inuse", "gauge", "number of pages in use", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.real_inuse; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_pgexct", "counter", "number of page faults", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.pgexct; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_pgins", "counter", "number of pages paged in", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.pgins; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_pgouts", "counter", "number of pages paged out", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.pgouts; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_pgspins", "counter", "number of page ins from paging space", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.pgspins; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_pgspouts", "counter", "number of page outs from paging space", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.pgspouts; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_scans", "counter", "number of page scans by clock", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.scans; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_cycles", "counter", "number of page replacement cycles", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.cycles; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_pgsteals", "counter", "number of page steals", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.pgsteals; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_numperm", "gauge", "number of frames used for files", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.numperm; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_numpgsp", "gauge", "number of pages with allocated paging space", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.numpgsp; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_real_system", "gauge", "number of pages used by system segments. This is the sum of all the used pages in segment marked for system usage. Since segment classifications are not always guaranteed to be accurate, This number is only an approximation.", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.real_system; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_real_user", "gauge", "number of pages used by non-system segments. This is the sum of all pages used in segments not marked for system usage. Since segment classifications are not always guaranteed to be accurate, This number is only an approximation.", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.real_user; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_real_process", "gauge", "number of pages used by process segments. This is real_total - real_free - numperm - real_system. Since real_system is an approximation, this number is too.", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.real_process; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_virt_active", "gauge", "Active virtual pages. Virtual pages are considered active if they have been accessed", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.virt_active; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_comprsd_total", "gauge", "Number of pages of this size compressed", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.comprsd_total; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_comprsd_wseg_pgs", "gauge", "Number of compressed working storage pages", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.comprsd_wseg_pgs; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_cpgins", "counter", "number of page ins of this page size to compressed pool", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.cpgins; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_cpgouts", "counter", "number of page outs of this page size from compressed pool", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.cpgouts; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_cpool_inuse", "gauge", "Compressed Size of this page size in Compressed Pool", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.cpool_inuse; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_ucpool_size", "gauge", "Uncompressed Pool size in bytes of this page size", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.ucpool_size; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_comprsd_wseg_size", "gauge", "Total size of the compressed working storage pages in the pool", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.comprsd_wseg_size; });
	output_memory_page_stat_mode(response, static_labels, "aix_memory_page_real_avail", "gauge", "number of pages (in 4KB pages) of memory available without paging out working segments", memory_pages, memory_page_count, [](perfstat_memory_page_t& memory_page) { return memory_page.real_avail; });
}
