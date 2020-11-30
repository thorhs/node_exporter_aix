#include <string>
#include <sstream>
#include <functional>

#include <libperfstat.h>
#include <stdio.h>
#include <strings.h>

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

	labels << "machine_serial=\"" << partition_config.machineID << "\",lpar=\"" << partition_config.partitionname << "\",group_id=\"" << partition_config.groupid << "\"";

	output_str = labels.str();

	return output_str;
}

void output_cpus_stat_mode(std::ostringstream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_cpu_t cpus[], size_t cpu_count, const std::function<double (perfstat_cpu_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " counter"  << std::endl;

	for(size_t i=0; i<cpu_count; i++) {
		response << name << "{cpu=\"cpu" << i << "\",mode=\"" << type << "\"} " << func(cpus[i]) << std::endl;
	}
}

void output_cpus_stat(std::ostringstream& response, const std::string& static_labels, const std::string& name, const std::string& help, perfstat_cpu_t cpus[], size_t cpu_count, const std::function<double (perfstat_cpu_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " counter"  << std::endl;

	for(size_t i=0; i<cpu_count; i++) {
		response << name << "{cpu=\"cpu" << i << "\"," << static_labels << "\"} " << func(cpus[i]) << std::endl;
	}
}

void gather_cpus_compat(std::ostringstream& response, const std::string& static_labels) {
	int cpu_count = perfstat_cpu(NULL, NULL, sizeof(perfstat_cpu_t), 0);

	perfstat_cpu_t cpus[cpu_count];
	perfstat_id_t firstcpu;

	strcpy(firstcpu.name, FIRST_CPU);

	if(perfstat_cpu(&firstcpu, cpus, sizeof(perfstat_cpu_t), cpu_count) <= 0) {
		perror("Error calling perfstat_cpu");
		return;
	}

	output_cpus_stat_mode(response, static_labels, "node_cpu", "user", "Seconds the cpus spent in each mode.", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return (double)cpu.user/100; });
	output_cpus_stat_mode(response, static_labels, "node_cpu", "idle", "Seconds the cpus spent in each mode.", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return (double)cpu.idle/100; });
	output_cpus_stat_mode(response, static_labels, "node_cpu", "sys",  "Seconds the cpus spent in each mode.", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return (double)cpu.sys/100;  });
	output_cpus_stat_mode(response, static_labels, "node_cpu", "wait", "Seconds the cpus spent in each mode.", cpus, cpu_count, [](perfstat_cpu_t& cpu) { return (double)cpu.wait/100; });
}


void output_cpu_stat(std::ostringstream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_cpu_total_t cpu, const std::function<double (perfstat_cpu_total_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	response << name << "{" << static_labels << "} " << func(cpu) << std::endl;
}

void gather_cpu_compat(std::ostringstream& response, const std::string& static_labels) {
	perfstat_cpu_total_t cpu;

	if(perfstat_cpu_total(NULL, &cpu, sizeof(perfstat_cpu_total_t), 1) <= 0) {
		perror("Error calling perfstat_cpu_total");
		return;
	}

	output_cpu_stat(response, static_labels, "node_load1",             "gauge",  "1m load average.",                     cpu, [](perfstat_cpu_total_t& cpu) { return (double)cpu.loadavg[0]/(1<< SBITS); });
	output_cpu_stat(response, static_labels, "node_load5",             "gauge",  "5m load average.",                     cpu, [](perfstat_cpu_total_t& cpu) { return (double)cpu.loadavg[1]/(1<< SBITS); });
	output_cpu_stat(response, static_labels, "node_load15",            "gauge",  "15m load average.",                    cpu, [](perfstat_cpu_total_t& cpu) { return (double)cpu.loadavg[2]/(1<< SBITS); });

	output_cpu_stat(response, static_labels, "node_context_switches", "counter", "Total number of context switches.",    cpu, [](perfstat_cpu_total_t& cpu) { return (double)cpu.pswitch; });
	output_cpu_stat(response, static_labels, "node_forks",            "counter", "Total number of forks.",               cpu, [](perfstat_cpu_total_t& cpu) { return (double)cpu.sysfork; });
	output_cpu_stat(response, static_labels, "node_intr",             "counter", "Total number of interrupts serviced.", cpu, [](perfstat_cpu_total_t& cpu) { return (double)cpu.decrintrs + (double)cpu.mpcrintrs + (double)cpu.mpcsintrs + (double)cpu.devintrs + (double)cpu.softintrs; });
}

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
