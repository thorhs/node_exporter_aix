void output_netinterface_stat_mode(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_netinterface_t netinterfaces[], size_t netinterface_count, const std::function<double (perfstat_netinterface_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	for(size_t i=0; i<netinterface_count; i++) {
		response << name << "{netinterface=\"" << netinterfaces[i].name << "\",";
                response << static_labels << "} " << func(netinterfaces[i]) << std::endl;
	}
}

void gather_netinterfaces(std::ostream& response, const std::string& static_labels) {
	std::cout << "interfaces metrics" << std::endl;

	int netinterface_count = perfstat_netinterface(NULL, NULL, sizeof(perfstat_netinterface_t), 0);

	perfstat_netinterface_t netinterfaces[netinterface_count];
	perfstat_id_t firstnetinterface;

	strcpy(firstnetinterface.name, FIRST_NETINTERFACE);

	if(perfstat_netinterface(&firstnetinterface, netinterfaces, sizeof(perfstat_netinterface_t), netinterface_count) <= 0) {
		perror("Error calling perfstat_netinterface");
		return;
	}

	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_type", "gauge", "ethernet, tokenring, etc. interpretation can be done using /usr/include/net/if_types.h", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.type; });
	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_mtu", "gauge", "network frame size", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.mtu; });
	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_ipackets", "counter", "number of packets received on interface", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.ipackets; });
	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_ibytes", "counter", "number of bytes received on interface", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.ibytes; });
	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_ierrors", "counter", "number of input errors on interface", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.ierrors; });
	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_opackets", "counter", "number of packets sent on interface", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.opackets; });
	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_obytes", "counter", "number of bytes sent on interface", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.obytes; });
	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_oerrors", "counter", "number of output errors on interface", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.oerrors; });
	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_collisions", "counter", "number of collisions on csma interface", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.collisions; });
	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_bitrate", "gauge", "adapter rating in bit per second", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.bitrate; });
	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_xmitdrops", "counter", "number of packets not transmitted", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.xmitdrops; });
	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_if_iqdrops", "counter", "Dropped on input, this interface", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.if_iqdrops; });
	output_netinterface_stat_mode(response, static_labels, "aix_netinterface_if_arpdrops", "counter", "Dropped because no arp response", netinterfaces, netinterface_count, [](perfstat_netinterface_t& netinterface) { return netinterface.if_arpdrops; });
}
