# Example grafana dashboard

server_overview_aix.json is an example Grafana dashboard, exported from my
internal grafana server.  I have been doing some major changes so I hope
everything is still working as it should.

# Sections

## Physical core utilization (SPURR)
This, hopefully displays the physical CPU utilization of the LPAR.  This
is the graph I'm most sceptical of since I'm guessing based on what I found
how this is calculated.  Take with a grain of salt.

![Physical core utilization (SPURR)](images/spurr.png?raw=true "Physical core utilization (SPURR)")

## Run Queue (Load)
This graph displays the run queue as reported by the kernel.

![Run Queue (Load)](images/load.png?raw=true "Run Queue (Load)")

## Active logical CPUs
This graph displays the number of currently allocated logical CPUs, or SMT
threads.

![Active logical CPUs](images/cpus.png?raw=true "Active logical CPUs")

## Disk queue length
This graph displays the current disk queue length for each disk on the
LPAR.  High values here indicate that the storage is not keeping up
with the demand of the LPAR.

![Disk queue length](images/disk_queue.png?raw=true "Disk queue length")

## Disk service time
This graph displays the average service time for each disk on the LPAR.
Positive values are reads, while writes are negative.  Service time
is the duration from the LPAR sending a request to disk to receiving
the response.

![Disk service time](images/disk_svc_time.png?raw=true "Disk service time")

## Worst case disk service time
AIX also tracks the worst case disk service time, that is what was the
longest AIX had to wait for a response from the disk.

![Worst case disk service time](images/disk_svc_worst.png?raw=true "Worst case disk service time")

## Accumulated wait queue items
This graph displays the number of times a request was put on the IO wait queue,
when the disk had too may outstanding IO requests.  Often related to the Disk
queue length graph above.

![Accumulated wait queue items](images/disk_wait_items.png?raw=true "Accumulated wait queue items")

## Max wait queue time
This graph displays the max time an item waited on the IO wait queue.

![Max wait queue time](images/disk_wait_max.png?raw=true "Max wait queue time")

## Disk transfers/s (IOPs)
This graph displays the number of IO requests are performed for each disk.

![Disk transfers/s (IOPs)](images/disk_iops.png?raw=true "Disk transfers/s (IOPs)")

## Disk throughput B/s
This graph displays the amount of data being sent to/read from each disk.

![Disk throughput B/s](images/disk_throughput.png?raw=true "Disk throughput B/s")

## Total disk throughput B/s, per VG
This graph displays the amount of data being sent to/read from each
volume group.

![Total disk throughput B/s, per VG](images/disk_throughput_vg.png?raw=true "Total disk throughput B/s, per VG")

## Total disk throughput B/s
This graph displays the amount of data being sent to/read for the whole LPAR.

![Total disk throughput B/s](images/disk_throughput_total.png?raw=true "Total disk throughput B/s")

## Free memory
This graph displays the total memory of the LPAR, and the inuse memory.  This
can give a clear indication as to how much memory is being consumed.


![Free memory](images/memory_free.png?raw=true "Free memory")

## Memory page scans
This garph shows the page scan activity of the system.  Page scans are
performed when the OS is trying to reclaim memory, by evicting cached
data etc.  This can indicate memory exhaustion.

![Memory page scans](images/memory_scans.png?raw=true "Memory page scans")

## Network adapter packets per second
This graphs shows the nubmer of packets being sent and received per second
by each network adapter.

![Network adapter packets per second](images/network_pps.png?raw=true "Network adapter packets per second")

## Network adapter bytes per second
This graphs shows the nubmer of bytes being sent and received per second
by each network adapter.

![Network adapter bytes per second](images/network_bps.png?raw=true "Network adapter bytes per second")

## Network adapter average packet size
This graphs shows the average packet size being sent and received for each
interval.  This is just a simple bps/pps calculation.

![Network adapter average packet size](images/network_psize.png?raw=true "Network adapter average packet size")

## Network errors
This graphs shows network error rates.

![Network errors](images/network_errors.png?raw=true "Network errors")
