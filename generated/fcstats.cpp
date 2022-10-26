void output_fcstat_stat_mode(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_fcstat_t fcstats[], size_t fcstat_count, const std::function<double (perfstat_fcstat_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	for(size_t i=0; i<fcstat_count; i++) {
		response << name << "{fcstat=\"" << fcstats[i].name << "\",";
		response << "PortWWN=\"" << std::hex <<  fcstats[i].PortWWN << std::dec  << "\",";
		response << "PortFcId=\"" << std::hex <<  fcstats[i].PortFcId << std::dec  << "\",";
                response << static_labels << "} " << func(fcstats[i]) << std::endl;
	}
}

void gather_fcstats(std::ostream& response, const std::string& static_labels) {
    std::cout << "fcstats metrics" << std::endl;
	int fcstat_count = perfstat_fcstat(NULL, NULL, sizeof(perfstat_fcstat_t), 0);

	perfstat_fcstat_t fcstats[fcstat_count];
	perfstat_id_t firstfcstat;

	strcpy(firstfcstat.name, FIRST_VFCHOST);

	if(perfstat_fcstat(&firstfcstat, fcstats, sizeof(perfstat_fcstat_t), fcstat_count) <= 0) {
		perror("Error calling perfstat_fcstat");
		return;
	}

	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_InputRequests", "counter", "Number of Input Requests", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.InputRequests; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_OutputRequests", "counter", "Number of Output Requests", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.OutputRequests; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_InputBytes", "counter", "Number of Input Bytes", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.InputBytes; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_OutputBytes", "counter", "Number of Output Bytes", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.OutputBytes; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_NoDMAResourceCnt", "counter", "Count of DMA failures due to no DMA Resource available", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.NoDMAResourceCnt; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_NoCmdResourceCnt", "counter", "Count of failures to allocate a command due to no command resource available", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.NoCmdResourceCnt; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_TxFrames", "counter", "Number of frames transmitted", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.TxFrames; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_TxWords", "counter", "Fiber Channel Kbytes transmitted", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.TxWords; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_RxFrames", "counter", "Number of Frames Received.", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.RxFrames; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_RxWords", "counter", "Fiber Channel Kbytes Received", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.RxWords; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_LIPCount", "counter", "Count of LIP(Loop Initialization Protocol) Events received in case we have FC-AL (arbitrated loop - a FC topology)", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.LIPCount; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_NOSCount", "counter", "Count of NOS(Not_Operational) Events. This  indicates a link failure state.  Count is number of times such events  occurred.  high level, this just indicates that we lost connection to FC network.", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.NOSCount; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_ErrorFrames", "counter", "Number of frames received with the CRC Error", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.ErrorFrames; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_DumpedFrames", "counter", "Number of lost frames", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.DumpedFrames; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_LinkFailureCount", "counter", "Count of Link failures", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.LinkFailureCount; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_LossofSyncCount", "counter", "Count of loss of sync", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.LossofSyncCount; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_LossofSignal", "counter", "Count of loss of Signal.", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.LossofSignal; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_PrimitiveSeqProtocolErrCount", "counter", "number of times a primitive sequence was in error.", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.PrimitiveSeqProtocolErrCount; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_InvalidTxWordCount", "counter", "Count of Invalid Transmission words received", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.InvalidTxWordCount; });
	output_fcstat_stat_mode(response, static_labels, "aix_fcstat_InvalidCRCCount", "counter", "Count of CRC Errors in a Received Frame", fcstats, fcstat_count, [](perfstat_fcstat_t& fcstat) { return fcstat.InvalidCRCCount; });
}
