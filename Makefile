GIT_VERSION := $(shell git --no-pager describe --tags --always --long | sed "s/v\(.*\)-\([0-9]*\)-.*/\1.\2/")

all: build/node_exporter_aix

build/node_exporter_aix: build/server.o build/collectors.o build/main.o
	g++ -Wall -Werror -fmax-errors=5 -fconcepts -std=c++17 -pthread -lperfstat -I/usr/local/include -I/usr/local/include/simple-web-server -DUSE_STANDALONE_ASIO -DASIO_STANDALONE -DASIO_HAS_PTHREADS -static-libgcc -Wl,-bstatic -lstdc++ -lgcc -Wl,-bdynamic -o build/node_exporter_aix build/server.o build/collectors.o build/main.o

build/server.o: server.cpp
	g++ -Wall -Werror -fmax-errors=5 -fconcepts -std=c++17 -pthread -lperfstat -I/usr/local/include -I/usr/local/include/simple-web-server -I Simple-Web-Server -I asio/asio/include -DUSE_STANDALONE_ASIO -DASIO_STANDALONE -DASIO_HAS_PTHREADS -c -o build/server.o server.cpp

build/main.o: main.cpp
	g++ -Wall -Werror -fmax-errors=5 -fconcepts -std=c++17 -pthread -lperfstat -I/usr/local/include -I/usr/local/include/simple-web-server -DUSE_STANDALONE_ASIO -DASIO_STANDALONE -DASIO_HAS_PTHREADS -D PROG_VERSION="\"$(GIT_VERSION)\"" -c -o build/main.o main.cpp

build/collectors.o: collectors.cpp generated/diskpaths.cpp generated/diskadapters.cpp generated/memory_pages.cpp generated/memory.cpp generated/cpus.cpp generated/disks.cpp generated/netinterfaces.cpp generated/netadapters.cpp generated/netbuffers.cpp generated/partition.cpp
	g++ -Wall -Werror -fmax-errors=5 -fconcepts -std=c++17 -pthread -lperfstat -I/usr/local/include -I/usr/local/include/simple-web-server -DUSE_STANDALONE_ASIO -DASIO_STANDALONE -DASIO_HAS_PTHREADS  -c -o build/collectors.o collectors.cpp

generated/%s.cpp: data_sources/%.multiple scripts/generate_multiple.ksh templates/generate_multiple.template
	ksh scripts/generate_multiple.ksh $* generated/$*s.cpp

generated/%.cpp: data_sources/%.total scripts/generate_total.ksh templates/generate_total.template
	ksh scripts/generate_total.ksh $* generated/$*.cpp

run: build/node_exporter_aix
	echo Starting
	build/node_exporter_aix

bff: clean build/node_exporter_aix
	sed "s/VERSION/$(GIT_VERSION)/; s%PWD%$(PWD)%;" bff_template > build/bff_template
	sudo rm -rf build/root
	mkdir -p build/root/usr/local/bin/
	cp build/node_exporter_aix build/root/usr/local/bin/
	sudo chown root.system build/root/usr/local/bin/node_exporter_aix
	sudo mkinstallp -d build/root -T build/bff_template
	cp build/root/tmp/node_exporter_aix.$(GIT_VERSION).bff build/

clean:
	sudo rm -rf build/* generated/*
