#include <chrono>
#include <map>
#include <string>
#include <thread>
#include <sstream>

#include "server_http.hpp"

#include <stdio.h>
#include <strings.h>
#include <memory>

#include "node_exporter_aix.hpp"

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;


int start_server(int port, int flags) {

	HttpServer server;
	server.config.port = port;

	server.default_resource["GET"] = [flags](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
		std::ostringstream output;

		auto static_labels = generate_static_labels();

		if(flags & PART_COMPAT)       gather_cpu_compat(output, static_labels);
		if(flags & PART_COMPAT)       gather_cpus_compat(output, static_labels);
		if(flags & PART_CPU)          gather_cpus(output, static_labels);
		if(flags & PART_DISKADAPTER)  gather_diskadapters(output, static_labels);
		if(flags & PART_DISKPATH)     gather_diskpaths(output, static_labels);
		if(flags & PART_MEM_PAGES)    gather_memory_pages(output, static_labels);
		if(flags & PART_MEM)          gather_memory(output, static_labels);
		if(flags & PART_DISK)         gather_disks(output, static_labels);
		if(flags & PART_NETINTERFACE) gather_netinterfaces(output, static_labels);
		if(flags & PART_NETADAPTER)   gather_netadapters(output, static_labels);
		if(flags & PART_NETBUFFER)    gather_netbuffers(output, static_labels);
		if(flags & PART_PARTITION)    gather_partition(output, static_labels);
		if(flags & PART_FILESYSTEMS)  gather_filesystems(output, static_labels);
		if(flags & PART_VMSTAT_V)     gather_vmstat_v(output, static_labels);
		if(flags & PART_FCSTAT_E)     gather_fcstats(output, static_labels);

		auto outstr = output.str();
		*response << "HTTP/1.1 200 OK\r\nContent-Length: " << outstr.length() << "\r\n\r\n"
			<< outstr;
	};

	std::thread server_thread([&server]() {
			// Start server
			server.start();
			});



	server_thread.join();


	return 0;
}

