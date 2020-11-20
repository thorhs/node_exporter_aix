#define PART_COMPAT       (1<<0)
#define PART_CPU          (1<<1)
#define PART_DISKADAPTER  (1<<2)
#define PART_DISKPATH     (1<<3)
#define PART_MEM_PAGES    (1<<4)
#define PART_MEM          (1<<5)
#define PART_DISK         (1<<6)
#define PART_NETINTERFACE (1<<7)
#define PART_NETADAPTER   (1<<8)
#define PART_NETBUFFER    (1<<9)
#define PART_PARTITION    (1<<20)


extern int start_server(int port, int flags);

extern std::string generate_static_labels();

extern void gather_cpus_compat(std::ostringstream& response, const std::string& static_labels);
extern void gather_cpu_compat(std::ostringstream& response, const std::string& static_labels);
extern void gather_diskadapters(std::ostringstream& response, const std::string& static_labels);
extern void gather_diskpaths(std::ostringstream& response, const std::string& static_labels);
extern void gather_memory_pages(std::ostringstream& response, const std::string& static_labels);
extern void gather_memory(std::ostringstream& response, const std::string& static_labels);
extern void gather_cpus(std::ostringstream& response, const std::string& static_labels);
extern void gather_disks(std::ostringstream& response, const std::string& static_labels);
extern void gather_netinterfaces(std::ostringstream& response, const std::string& static_labels);
extern void gather_netadapters(std::ostringstream& response, const std::string& static_labels);
extern void gather_netbuffers(std::ostringstream& response, const std::string& static_labels);
extern void gather_partition(std::ostringstream& response, const std::string& static_labels);
