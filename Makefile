GIT_VERSION := $(shell git --no-pager describe --tags --always --long | sed "s/v\(.*\)-\([0-9]*\)-.*/\1.\2/")

all: build/node_exporter_aix

build/node_exporter_aix: build/server.o build/collectors.o build/main.o build/mounts.o build/vmstat_v.o
	g++ -Wall -Werror -fmax-errors=5 -fconcepts -std=c++17 -pthread -lperfstat -DUSE_STANDALONE_ASIO -DASIO_STANDALONE -DASIO_HAS_PTHREADS -static-libgcc -Wl,-bstatic -lstdc++ -lgcc -Wl,-bdynamic -o build/node_exporter_aix build/server.o build/collectors.o build/main.o build/mounts.o build/vmstat_v.o

build/server.o: server.cpp node_exporter_aix.hpp
	g++ -Wall -Werror -fmax-errors=5 -fconcepts -std=c++17 -pthread -lperfstat -I Simple-Web-Server -I asio/asio/include -DUSE_STANDALONE_ASIO -DASIO_STANDALONE -DASIO_HAS_PTHREADS -c -o build/server.o server.cpp

build/main.o: main.cpp node_exporter_aix.hpp
	g++ -Wall -Werror -fmax-errors=5 -fconcepts -std=c++17 -pthread -lperfstat -DUSE_STANDALONE_ASIO -DASIO_STANDALONE -DASIO_HAS_PTHREADS -D PROG_VERSION="\"$(GIT_VERSION)\"" -c -o build/main.o main.cpp

build/mounts.o: mounts.cpp node_exporter_aix.hpp
	g++ -Wall -Werror -fmax-errors=5 -fconcepts -std=c++17 -pthread -lperfstat -DUSE_STANDALONE_ASIO -DASIO_STANDALONE -DASIO_HAS_PTHREADS -D PROG_VERSION="\"$(GIT_VERSION)\"" -c -o build/mounts.o mounts.cpp

build/vmstat_v.o: vmstat_v.cpp
	g++ -Wall -Werror -fmax-errors=5 -fconcepts -std=c++17 -pthread -lperfstat -DUSE_STANDALONE_ASIO -DASIO_STANDALONE -DASIO_HAS_PTHREADS -D PROG_VERSION="\"$(GIT_VERSION)\"" -c -o build/vmstat_v.o vmstat_v.cpp

build/collectors.o: collectors.cpp generated/diskpaths.cpp generated/diskadapters.cpp generated/memory_pages.cpp generated/memory.cpp generated/cpus.cpp generated/disks.cpp generated/netinterfaces.cpp generated/netadapters.cpp generated/netbuffers.cpp generated/partition.cpp generated/fcstats.cpp node_exporter_aix.hpp
	g++ -Wall -Werror -fmax-errors=5 -fconcepts -std=c++17 -pthread -lperfstat -DUSE_STANDALONE_ASIO -DASIO_STANDALONE -DASIO_HAS_PTHREADS  -c -o build/collectors.o collectors.cpp

generated/%s.cpp: data_sources/%.multiple scripts/generate_multiple.ksh templates/generate_multiple.template
	ksh scripts/generate_multiple.ksh $* generated/$*s.cpp

generated/%.cpp: data_sources/%.total scripts/generate_total.ksh templates/generate_total.template
	ksh scripts/generate_total.ksh $* generated/$*.cpp

run: build/node_exporter_aix
	echo Starting
	build/node_exporter_aix

bff: clean build/node_exporter_aix
	sed "s/VERSION/$(GIT_VERSION)/; s%PWD%$(PWD)%;" bff_template > build/bff_template
	rm -rf build/root
	mkdir -p build/root/usr/local/bin/
	cp build/node_exporter_aix build/root/usr/local/bin/
	chown root.system build/root/usr/local/bin/node_exporter_aix
	mkinstallp -d build/root -T build/bff_template
	cp build/root/tmp/node_exporter_aix.$(GIT_VERSION).bff build/

clean:
	rm -rf build/* generated/*
