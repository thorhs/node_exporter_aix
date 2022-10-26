void output_netbuffer_stat_mode(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_netbuffer_t netbuffers[], size_t netbuffer_count, const std::function<double (perfstat_netbuffer_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	for(size_t i=0; i<netbuffer_count; i++) {
		response << name << "{netbuffer=\"" << netbuffers[i].name << "\",";
                response << static_labels << "} " << func(netbuffers[i]) << std::endl;
	}
}

void gather_netbuffers(std::ostream& response, const std::string& static_labels) {
    std::cout << "netbuffer metrics" << std::endl;

    int netbuffer_count = perfstat_netbuffer(NULL, NULL, sizeof(perfstat_netbuffer_t), 0);

	perfstat_netbuffer_t netbuffers[netbuffer_count];
	perfstat_id_t firstnetbuffer;

	strcpy(firstnetbuffer.name, FIRST_NETBUFFER);

	if(perfstat_netbuffer(&firstnetbuffer, netbuffers, sizeof(perfstat_netbuffer_t), netbuffer_count) <= 0) {
		perror("Error calling perfstat_netbuffer");
		return;
	}

	output_netbuffer_stat_mode(response, static_labels, "aix_netbuffer_inuse", "gauge", "number of buffer currently allocated", netbuffers, netbuffer_count, [](perfstat_netbuffer_t& netbuffer) { return netbuffer.inuse; });
	output_netbuffer_stat_mode(response, static_labels, "aix_netbuffer_calls", "counter", "number of buffer allocations since last reset", netbuffers, netbuffer_count, [](perfstat_netbuffer_t& netbuffer) { return netbuffer.calls; });
	output_netbuffer_stat_mode(response, static_labels, "aix_netbuffer_delayed", "counter", "number of delayed allocations", netbuffers, netbuffer_count, [](perfstat_netbuffer_t& netbuffer) { return netbuffer.delayed; });
	output_netbuffer_stat_mode(response, static_labels, "aix_netbuffer_free", "counter", "number of free calls", netbuffers, netbuffer_count, [](perfstat_netbuffer_t& netbuffer) { return netbuffer.free; });
	output_netbuffer_stat_mode(response, static_labels, "aix_netbuffer_failed", "counter", "number of failed allocations", netbuffers, netbuffer_count, [](perfstat_netbuffer_t& netbuffer) { return netbuffer.failed; });
	output_netbuffer_stat_mode(response, static_labels, "aix_netbuffer_highwatermark", "gauge", "high threshold for number of buffer allocated", netbuffers, netbuffer_count, [](perfstat_netbuffer_t& netbuffer) { return netbuffer.highwatermark; });
	output_netbuffer_stat_mode(response, static_labels, "aix_netbuffer_freed", "counter", "umber of buffers freed", netbuffers, netbuffer_count, [](perfstat_netbuffer_t& netbuffer) { return netbuffer.freed; });
}
