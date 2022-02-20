#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>
#include <algorithm>
#include <unordered_map>

struct vmstat_v_metric {
	std::string name;
	std::string type;
	std::string help;
};


std::string exec(const char* cmd) {
    std::array<char, 2048> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void output_vmstat_v_metric(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, const std::string value) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	response << name << "{" << static_labels << "} " << value << std::endl;
}

static const std::unordered_map<std::string, vmstat_v_metric> vmstat_v_metrics({
		{"compressed percentage",
			vmstat_v_metric{"aix_compressed_percentage", "gauge", "Percentage of memory used by compressed pages"}
		},
		{"client filesystem I/Os blocked with no fsbuf",
			vmstat_v_metric{"aix_client_fs_no_fsbuf", "counter", "Number of client filesystem I/O requests blocked because no fsbuf was available. NFS (Network File System) and VxFS (Veritas) are client filesystems. Fsbuf are pinned memory buffers used to hold I/O requests in the filesystem layer"}},
		{"client pages",
			vmstat_v_metric{"aix_client_pages", "gauge", "Number of client pages"}},
		{"compressed pages",
			vmstat_v_metric{"aix_compressed_pages", "gauge", "Number of compressed memory pages"}},
		{"external pager filesystem I/Os blocked with no fsbuf",
			vmstat_v_metric{"aix_ext_pager_fs_no_fsbuf", "counter", "Number of external pager client filesystem I/O requests blocked because no fsbuf was available. JFS2 is an external pager client filesystem. Fsbuf are pinned memory buffers used to hold I/O requests in the filesystem layer"}},
		{"file pages",
			vmstat_v_metric{"aix_file_pages", "counter", "Number of 4 KB pages that are currently used by the file cache"}},
		{"free pages",
			vmstat_v_metric{"aix_free_pages", "counter", "Number of free 4 KB pages"}},
		{"filesystem I/Os blocked with no fsbuf",
			vmstat_v_metric{"aix_fs_no_fsbuf", "counter", "Number of filesystem I/O requests blocked because no fsbuf was available. Fsbuf are pinned memory buffers used to hold I/O requests in the filesystem layer"}},
		{"lruable pages",
			vmstat_v_metric{"aix_lruable_pages", "gauge", "Number of 4 KB pages that are considered for replacement. This number excludes the pages that are used for VMM internal pages, and the pages that are used for the pinned part of the kernel text"}},
		{"maxclient percentage",
			vmstat_v_metric{"aix_maxclient_pct", "gauge", "Tuning parameter (managed using vmo) specifying the maximum percentage of memory, which can be used for client pages"}},
		{"maxperm percentage",
			vmstat_v_metric{"aix_maxperm_pct", "gauge", "Tuning parameter (managed using vmo) in percentage of real memory"}},
		{"maxpin percentage",
			vmstat_v_metric{"aix_maxpin_pct", "gauge", "Tuning parameter (managed using vmo) specifying the percentage of real memory which can be pinned"}},
		{"memory pages",
			vmstat_v_metric{"aix_memory_pages", "gauge", "Size of real memory in number of 4 KB pages"}},
		{"memory pools",
			vmstat_v_metric{"aix_memory_pools", "gauge", "Tuning parameter (managed using vmo) specifying the number of memory pools"}},
		{"minperm percentage",
			vmstat_v_metric{"aix_minperm_pct", "gauge", "Tuning parameter (managed using vmo) in percentage of real memory"}},
		{"numclient percentage",
			vmstat_v_metric{"aix_numclient_pct", "gauge", "Percentage of memory that is occupied by client pages"}},
		{"numperm percentage",
			vmstat_v_metric{"aix_numperm_pct", "gauge", "Tuning parameter (managed using vmo) in percentage of real memory"}},
		{"pinned pages",
			vmstat_v_metric{"aix_pinned_pages", "gauge", "Number of pinned pages"}},
		{"remote pageouts scheduled",
			vmstat_v_metric{"aix_remote_pageouts_scheduled", "counter", "Number of remote pageouts scheduled"}},
		{"pending disk I/Os blocked with no pbuf",
			vmstat_v_metric{"aix_pending_disk_io_no_pbuf", "counter", "Pending disk I/O blocked with no pbuf"}},
		{"paging space I/Os blocked with no psbuf",
			vmstat_v_metric{"aix_pagingspace_io_no_pbuf", "counter", "Paging space I/Os blocked with no psbuf"}},
		{"percentage of memory used for computational pages",
			vmstat_v_metric{"aix_pct_mem_compute_pages", "counter", "Percentage of memory used for computational pages"}}
		});

void gather_vmstat_v(std::ostream& response, const std::string& static_labels) {
	static const std::string WHITESPACE = " \n\r\t\f\v";

	std::string output = exec("vmstat -v");
	std::stringstream ss(output);
	std::string to;

	while(std::getline(ss, to, '\n')) {
		size_t start = to.find_first_not_of(WHITESPACE);
		if (start == std::string::npos) continue; // No data found

		size_t delim = to.find_first_of(WHITESPACE, start);
		if (delim == std::string::npos) continue; // No data found

		std::string v = to.substr(start, delim - start);
		std::string k = to.substr(delim + 1);

		auto metric = vmstat_v_metrics.find(k);

		if (metric == vmstat_v_metrics.end()) {
			std::cerr << "Unable to find '" << k << "' in vmstat_v_metrics" << std::endl;
			continue;
		}

		auto m = metric->second;

		output_vmstat_v_metric(response, static_labels, m.name, m.type, m.help, v);
	}
}

#if 0
int main() {

	gather_vmstat_v(std::cout, "static_labels");

	return 0;
}
#endif
