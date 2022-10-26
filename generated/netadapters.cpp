void output_netadapter_stat_mode(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_netadapter_t netadapters[], size_t netadapter_count, const std::function<double (perfstat_netadapter_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	for(size_t i=0; i<netadapter_count; i++) {
		response << name << "{netadapter=\"" << netadapters[i].name << "\",";
                response << static_labels << "} " << func(netadapters[i]) << std::endl;
	}
}

void gather_netadapters(std::ostream& response, const std::string& static_labels) {
    std::cout << "net adapters metrics" << std::endl;

    int netadapter_count = perfstat_netadapter(NULL, NULL, sizeof(perfstat_netadapter_t), 0);

	perfstat_netadapter_t netadapters[netadapter_count];
	perfstat_id_t firstnetadapter;

	strcpy(firstnetadapter.name, FIRST_NETADAPTER);

	if(perfstat_netadapter(&firstnetadapter, netadapters, sizeof(perfstat_netadapter_t), netadapter_count) <= 0) {
		perror("Error calling perfstat_netadapter");
		return;
	}

	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_packets", "counter", "Transmit Packets on interface", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_packets; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_bytes", "counter", "Transmit Bytes on interface", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_bytes; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_interrupts", "counter", "Transfer Interrupts", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_interrupts; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_errors", "counter", "Transmit Errors", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_errors; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_packets_dropped", "counter", "Packets Dropped at the time of Data Transmission", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_packets_dropped; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_queue_size", "counter", "Maximum Packets on Software Transmit Queue", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_queue_size; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_queue_len", "gauge", "Transmission Queue Length", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_queue_len; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_queue_overflow", "counter", "Transmission Queue Overflow", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_queue_overflow; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_broadcast_packets", "counter", "Number of Broadcast Packets Transmitted", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_broadcast_packets; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_multicast_packets", "counter", "Number of Multicast packets Transmitted", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_multicast_packets; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_carrier_sense", "counter", "Lost Carrier Sense signal count", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_carrier_sense; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_DMA_underrun", "counter", "Count of DMA Under-runs for Transmission", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_DMA_underrun; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_lost_CTS_errors", "counter", "The number of unsuccessful transmissions due to the loss of the Clear-to-Send signal error", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_lost_CTS_errors; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_max_collision_errors", "counter", "Maximum Collision Errors at Transmission", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_max_collision_errors; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_late_collision_errors", "counter", "Late Collision Errors at Transmission", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_late_collision_errors; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_deferred", "counter", "The number of packets deferred for Transmission.", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_deferred; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_timeout_errors", "counter", "Time Out Errors for Transmission", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_timeout_errors; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_single_collision_count", "counter", "Count of Single Collision error at Transmission", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_single_collision_count; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_tx_multiple_collision_count", "counter", "Count of Multiple Collision error at Transmission", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.tx_multiple_collision_count; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_packets", "counter", "Receive Packets on interface", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_packets; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_bytes", "counter", "Receive Bytes on interface", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_bytes; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_interrupts", "counter", "Receive Interrupts", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_interrupts; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_errors", "counter", "Input errors on interface", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_errors; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_packets_dropped", "counter", "The number of packets accepted by the device driver for transmission which were not (for any reason) given to the device.", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_packets_dropped; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_bad_packets", "counter", "Count of Bad Packets Received.", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_bad_packets; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_multicast_packets", "counter", "Number of MultiCast Packets Received", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_multicast_packets; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_broadcast_packets", "counter", "Number of Broadcast Packets Received", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_broadcast_packets; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_CRC_errors", "counter", "Count of Packets Received with CRC errors", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_CRC_errors; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_DMA_overrun", "counter", "Count of DMA over-runs for Data Receival.", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_DMA_overrun; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_alignment_errors", "counter", "Packets Received with Alignment Error", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_alignment_errors; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_noresource_errors", "counter", "Packets Received with No Resource Errors", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_noresource_errors; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_collision_errors", "counter", "Packets Received with Collision errors", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_collision_errors; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_packet_tooshort_errors", "counter", "Count of Short Packets Received.", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_packet_tooshort_errors; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_packet_toolong_errors", "counter", "Count of Too Long Packets Received.", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_packet_toolong_errors; });
	output_netadapter_stat_mode(response, static_labels, "aix_netadapter_rx_packets_discardedbyadapter", "counter", "Count of Received Packets discarded by Adapter.", netadapters, netadapter_count, [](perfstat_netadapter_t& netadapter) { return netadapter.rx_packets_discardedbyadapter; });
}
