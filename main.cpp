#include <string>
#include <iostream>

#include <stdio.h>
#include <strings.h>
#include <memory>
#include <unistd.h>

#include "node_exporter_aix.hpp"

#ifndef PROG_VERSION
#define PROG_VERSION "master"
#endif

int usage(int c) {
	std::cout << "node_exporter_aix [flags]" << std::endl;
	std::cout << "Flags:" << std::endl;
	std::cout << "  -p <port> TCP port to listen on, defaults to 9100" << std::endl;
	std::cout << std::endl;
	std::cout << "  -C Compat statistics, emulate linux node_exporter" << std::endl;
	std::cout << "  -c CPU statistics" << std::endl;
	std::cout << "  -A disk adapter statistics" << std::endl;
	std::cout << "  -D disk path statistics" << std::endl;
	std::cout << "  -M memory page statistics" << std::endl;
	std::cout << "  -m memory statistics" << std::endl;
	std::cout << "  -d disk statistics" << std::endl;
	std::cout << "  -i net interface statistics" << std::endl;
	std::cout << "  -a net adapter statistics" << std::endl;
	std::cout << "  -b net buffer statistics" << std::endl;
	std::cout << "  -p partition statistics" << std::endl;

	return 1;
}

int main(int argc, char **argv) {
	int c;
	// extern int optind;
	extern char *optarg;

	int flags = 0, port = 9100;

	while((c = getopt(argc, argv, "p:CcAPMmdiabp")) != EOF) {
		switch(c) {
			// gather_cpus_compat(output);
			case 'C': flags |= PART_COMPAT; break;
			case 'c': flags |= PART_CPU; break;
			case 'A': flags |= PART_DISKADAPTER; break;
			case 'D': flags |= PART_DISKPATH; break;
			case 'M': flags |= PART_MEM_PAGES; break;
			case 'm': flags |= PART_MEM; break;
			case 'd': flags |= PART_DISK; break;
			case 'i': flags |= PART_NETINTERFACE; break;
			case 'a': flags |= PART_NETADAPTER; break;
			case 'b': flags |= PART_NETBUFFER; break;
			case 'P': flags |= PART_PARTITION; break;
			case 'p': port = std::stoi(optarg); break;
			default:
				  return usage(c);
		}
	}

	if(flags == 0) flags=0xffffffff;

	std::cout << "Node exporter for AIX version " PROG_VERSION " listening on port " << port << std::endl;

	return start_server(port, flags);
}

