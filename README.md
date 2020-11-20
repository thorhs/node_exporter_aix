# Node exporter for AIX

Prometheus exporter for hardware and OS metrics exposed by AIX, written
in C++ utilizing the libperfstat library supplied with AIX.

## Collectors

There is varying support for various subsystems. The table below
below list all existing collectors.

All collectors are enabled by default.  If collectors are specified on the
command line, only thoes collectors will be enabled.

## Default port
Default port used by this node exporter is :9100.

The port can be changed with the -p flag:

    ./build/node_exporter_aix -p 9101

### Enabled by default

flag | Description
-----|-------------
-C | Compat statistics, emulate linux node_exporter
-c | CPU statistics
-A | disk adapter statistics
-D | disk path statistics
-M | memory page statistics
-m | memory statistics
-d | disk statistics
-i | net interface statistics
-a | net adapter statistics
-b | net buffer statistics
-p | partition statistics
## Building and running

Prerequisites:

* AIX 7.1 (older may work, not tested)
* GCC C++17 compatible compiler (AIX Toolbox)
* GNU Make (AIX Toolbox)

* bos.adt.insttools (lpp, for packaging as .bff)

Building:

    export PATH=/opt/freeware/bin:$PATH # If required, to use GNU make
    git submodule init
    git submodule update
    make
    ./build/node_exporter_aix

To see all available configuration flags:

    ./build/node_exporter_aix -h
    
# Contributing
Please see [CONTRIBUTING.md](CONTRIBUTING.md).  We welcome issues, pull requests and general suggestions.

# License
This program is distributed under the terms of the MIT license.

See [LICENSE](LICENSE) for details.
