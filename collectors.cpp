#include <string>
#include <sstream>
#include <functional>
#include <iomanip>

#include <libperfstat.h>
#include <stdio.h>
#include <strings.h>

#include "node_exporter_aix.hpp"

std::string generate_static_labels() {
	std::ostringstream labels;
	static std::string output_str;
	
	if (output_str.length() > 0) return output_str;

	// perfstat_partition_total_t partition;
	perfstat_partition_config_t partition_config;

	/*
	if(perfstat_partition_total(NULL, &partition, sizeof(perfstat_partition_total_t), 1) <= 0) {
		perror("Error calling perfstat_partition_total");
		return "";
	}
	*/

	if(perfstat_partition_config(NULL, &partition_config, sizeof(perfstat_partition_config_t), 1) <= 0) {
		perror("Error calling perfstat_partition_config");
		return "";
	}

	labels  << "machine_serial=\"" << partition_config.machineID
		<< "\",lpar=\"" << partition_config.partitionname
		<< "\",cpupool_id=\"" << partition_config.processor_poolid
		<< "\",group_id=\"" << partition_config.groupid
		<< "\"";

	output_str = labels.str();

	return output_str;
}

void output_cpus_stat_mode(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_cpu_t *cpus, size_t cpu_count, const std::function<u_longlong_t (perfstat_cpu_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " counter"  << std::endl;

	for(size_t i=0; i<cpu_count; i++) {
		response << name << "{cpu=\"cpu" << i << "\",mode=\"" << type << "\"} " << std::fixed << std::setprecision(0) << func(cpus[i]) << std::endl;
	}
}

void output_cpus_stat(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& help, perfstat_cpu_t cpus[], size_t cpu_count, const std::function<u_longlong_t (perfstat_cpu_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " counter"  << std::endl;

	for(size_t i=0; i<cpu_count; i++) {
		response << name << "{cpu=\"cpu" << i << "\"," << static_labels << "\"} " << std::fixed << std::setprecision(0) << func(cpus[i]) << std::endl;
	}
}

void gather_cpus_compat(std::ostream& response, const std::string& static_labels) {
	perfstat_cpu_t *cpus;
	int cpu_count = perfstat_cpu(NULL, NULL, sizeof(perfstat_cpu_t), 0);
	cpus = (perfstat_cpu_t*)calloc(cpu_count, sizeof(perfstat_cpu_t));
	//perfstat_cpu_t cpus[cpu_count];
	perfstat_id_t firstcpu;

	strcpy(firstcpu.name, FIRST_CPU);

	if(perfstat_cpu(&firstcpu, cpus, sizeof(perfstat_cpu_t), cpu_count) <= 0) {
		perror("Error calling perfstat_cpu");
		return;
	}

	output_cpus_stat_mode(response, static_labels, "node_cpu", "user", "Seconds the cpus spent in each mode.", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return (u_longlong_t)cpu.user/100; });
	output_cpus_stat_mode(response, static_labels, "node_cpu", "idle", "Seconds the cpus spent in each mode.", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return (u_longlong_t)cpu.idle/100; });
	output_cpus_stat_mode(response, static_labels, "node_cpu", "sys",  "Seconds the cpus spent in each mode.", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return (u_longlong_t)cpu.sys/100;  });
	output_cpus_stat_mode(response, static_labels, "node_cpu", "wait", "Seconds the cpus spent in each mode.", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return (u_longlong_t)cpu.wait/100; });
	free(cpus);
}


void output_cpu_stat(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_cpu_total_t cpu, const std::function<u_longlong_t (perfstat_cpu_total_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	response << name << "{" << static_labels << "} " << std::fixed << std::setprecision(0) << func(cpu) << std::endl;
}

void gather_cpu_compat(std::ostream& response, const std::string& static_labels) {
	perfstat_cpu_total_t cpu;

	if(perfstat_cpu_total(NULL, &cpu, sizeof(perfstat_cpu_total_t), 1) <= 0) {
		perror("Error calling perfstat_cpu_total");
		return;
	}

	output_cpu_stat(response, static_labels, "node_load1",             "gauge",  "1m load average.",                     cpu, [](perfstat_cpu_total_t& cpu) { return (u_longlong_t)cpu.loadavg[0]/(1<< SBITS); });
	output_cpu_stat(response, static_labels, "node_load5",             "gauge",  "5m load average.",                     cpu, [](perfstat_cpu_total_t& cpu) { return (u_longlong_t)cpu.loadavg[1]/(1<< SBITS); });
	output_cpu_stat(response, static_labels, "node_load15",            "gauge",  "15m load average.",                    cpu, [](perfstat_cpu_total_t& cpu) { return (u_longlong_t)cpu.loadavg[2]/(1<< SBITS); });

	output_cpu_stat(response, static_labels, "node_context_switches", "counter", "Total number of context switches.",    cpu, [](perfstat_cpu_total_t& cpu) { return (u_longlong_t)cpu.pswitch; });
	output_cpu_stat(response, static_labels, "node_forks",            "counter", "Total number of forks.",               cpu, [](perfstat_cpu_total_t& cpu) { return (u_longlong_t)cpu.sysfork; });
	output_cpu_stat(response, static_labels, "node_intr",             "counter", "Total number of interrupts serviced.", cpu, [](perfstat_cpu_total_t& cpu) { return (u_longlong_t)cpu.decrintrs + (u_longlong_t)cpu.mpcrintrs + (u_longlong_t)cpu.mpcsintrs + (u_longlong_t)cpu.devintrs + (u_longlong_t)cpu.softintrs; });
}

void gather_filesystems(std::ostream& response, const std::string& static_labels) {
	std::vector<filesystem> filesystems = stat_filesystems(list_mounts());

	response << "# HELP node_filesystem_size_bytes Filesystem size in bytes." << std::endl;
	response << "# TYPE node_filesystem_size_bytes gauge" << std::endl;
	for(auto it = filesystems.begin(); it < filesystems.end(); it++) {
		response << "node_filesystem_size_bytes{device=\"" << (*it).device << "\",fstype=\"" << (*it).fstype << "\",mountpoint=\"" << (*it).mountpoint << "\"," << static_labels << "} " << std::fixed << std::setprecision(0) << (*it).size_bytes << std::endl;
	}

	response << "# HELP node_filesystem_free_bytes Filesystem free space in bytes." << std::endl;
	response << "# TYPE node_filesystem_free_bytes gauge" << std::endl;
	for(auto it = filesystems.begin(); it < filesystems.end(); it++) {
		response << "node_filesystem_free_bytes{device=\"" << (*it).device << "\",fstype=\"" << (*it).fstype << "\",mountpoint=\"" << (*it).mountpoint << "\"," << static_labels << "} " << std::fixed << std::setprecision(0) << (*it).free_bytes << std::endl;
	}

	response << "# HELP node_filesystem_avail_bytes Filesystem space available to non-root users in bytes." << std::endl;
	response << "# TYPE node_filesystem_avail_bytes gauge" << std::endl;
	for(auto it = filesystems.begin(); it < filesystems.end(); it++) {
		response << "node_filesystem_avail_bytes{device=\"" << (*it).device << "\",fstype=\"" << (*it).fstype << "\",mountpoint=\"" << (*it).mountpoint << "\"," << static_labels << "} " << std::fixed << std::setprecision(0) << (*it).avail_bytes << std::endl;
	}

	response << "# HELP node_filesystem_files Filesystem total file nodes." << std::endl;
	response << "# TYPE node_filesystem_files gauge" << std::endl;
	for(auto it = filesystems.begin(); it < filesystems.end(); it++) {
		response << "node_filesystem_files{device=\"" << (*it).device << "\",fstype=\"" << (*it).fstype << "\",mountpoint=\"" << (*it).mountpoint << "\"," << static_labels << "} " << std::fixed << std::setprecision(0) << (*it).files << std::endl;
	}

	response << "# HELP node_filesystem_files_free Filesystem total free file nodes." << std::endl;
	response << "# TYPE node_filesystem_files_free gauge" << std::endl;
	for(auto it = filesystems.begin(); it < filesystems.end(); it++) {
		response << "node_filesystem_files_free{device=\"" << (*it).device << "\",fstype=\"" << (*it).fstype << "\",mountpoint=\"" << (*it).mountpoint << "\"," << static_labels << "} " << std::fixed << std::setprecision(0) << (*it).files_free << std::endl;
	}

	response << "# HELP node_filesystem_files_avail Filesystem available file nodes to non-root users." << std::endl;
	response << "# TYPE node_filesystem_files_avail gauge" << std::endl;
	for(auto it = filesystems.begin(); it < filesystems.end(); it++) {
		response << "node_filesystem_files_avail{device=\"" << (*it).device << "\",fstype=\"" << (*it).fstype << "\",mountpoint=\"" << (*it).mountpoint << "\"," << static_labels << "} " << std::fixed << std::setprecision(0) << (*it).files_avail << std::endl;
	}
}

/*
// Workaround the fact that the structure and function are not called the same
int perfstat_fcstat(perfstat_id_t *name,
                      perfstat_diskadapter_t* userbuff,
                      int sizeof_userbuff,
                      int desired_number) {
	perfstat_virtualdiskadapter(name, userbuff, sizeof_userbuff, desired_number);
}
*/

#include "generated/diskadapters.cpp"
#include "generated/diskpaths.cpp"
#include "generated/memory_pages.cpp"
#include "generated/memory.cpp"
#include "generated/cpus.cpp"
#include "generated/disks.cpp"
#include "generated/netinterfaces.cpp"
#include "generated/netadapters.cpp"
#include "generated/netbuffers.cpp"
#include "generated/partition.cpp"
#include "generated/fcstats.cpp"
