#!/bin/sh

if lssrc -s node_exporter_aix > /dev/null 2>&1; then
	chssys -p /usr/local/bin/node_exporter_aix -s node_exporter_aix -u root -R -S -f 9 -n 15
else
	mkssys -p /usr/local/bin/node_exporter_aix -s node_exporter_aix -u root -R -S -f 9 -n 15
fi
