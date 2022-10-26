#!/bin/sh

if lssrc -s node_exporter_aix | grep -q active > /dev/null 2>&1; then
	stopsrc -s node_exporter_aix
fi
