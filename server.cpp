#include <chrono>
#include <map>
#include <string>
#include <thread>
#include <sstream>

#include "server_http.hpp"

#include <stdio.h>
#include <strings.h>
#include <memory>
#include <iostream>


#include "node_exporter_aix.hpp"

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
std::mutex myMutex;

int start_server(int port, int flags) {

	HttpServer server;
    server.on_error = [](std::shared_ptr<HttpServer::Request> request, const SimpleWeb::error_code ec) {
        // 117 is normal because of the async nature
        if(ec.value() == 117 || ec.value() == 2) {
            return;
        }
        std::cout << "error encountered "  << ec.message() << " "  << " "  << ec.value() << std::endl;
        std::cout << "error encountered with request" << request->content.string()  << std::endl;

    };
    server.config.port = port;
	server.default_resource["GET"] = [flags](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
        std::cout << "starting request for metrics" << std::endl;

        std::lock_guard<std::mutex> guard(myMutex);
		std::ostringstream output;
        std::cout << "generating static labels" << std::endl;

		auto static_labels = generate_static_labels();
        std::cout << "generated static labels" << std::endl;
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
			try {
                std::cout << "Starting server thread" << std::endl;
                server.start();
			} catch (const SimpleWeb::system_error &e) {
				std::cerr << "Error starting HTTP server: " << e.what() << std::endl;
			}
			});



	server_thread.join();


	return 0;
}

