#!/bin/sh

if lssrc -s node_exporter_aix > /dev/null 2>&1; then
	stopsrc -s node_exporter_aix
	rmssys -s node_exporter_aix
fi
