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

build/collectors.o: collectors.cpp node_exporter_aix.hpp
	g++ -Wall -Werror -fmax-errors=5 -fconcepts -std=c++17 -pthread -lperfstat -DUSE_STANDALONE_ASIO -DASIO_STANDALONE -DASIO_HAS_PTHREADS  -c -o build/collectors.o collectors.cpp

run: build/node_exporter_aix
	echo Starting
	build/node_exporter_aix

bff: build/node_exporter_aix
	sed "s/VERSION/$(GIT_VERSION)/; s%PWD%$(PWD)%;" bff_template > build/bff_template
	sudo rm -rf build/root
	mkdir -p build/root/usr/local/bin/
	cp build/node_exporter_aix build/root/usr/local/bin/
	sudo chown root.system build/root/usr/local/bin/node_exporter_aix
	sudo mkinstallp -d build/root -T build/bff_template
	cp build/root/tmp/node_exporter_aix.$(GIT_VERSION).bff build/
