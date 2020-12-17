#include <sys/types.h>
#include <sys/mntctl.h>
#include <sys/vmount.h>
#include <sys/statvfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>

#include "node_exporter_aix.hpp"

std::vector<mountpoint> list_mounts() {
	std::vector<mountpoint> output;

	char *buffer, *current;

	current = buffer = (char *)malloc(102400);
	memset(buffer, 0, 102400);

	int rc;
	rc = mntctl(MCTL_QUERY, 102400, buffer);

	if(rc<0) {
		perror("Error getting initial");
		exit(0);
	}

	struct vmount *mounts;

	for(int i=0; i<rc; i++) {
		mounts = (struct vmount *)current;

		if(mounts->vmt_gfstype == MNT_J2 || mounts->vmt_gfstype == MNT_JFS) {
			if(vmt2datasize(mounts, VMT_STUB) && vmt2datasize(mounts, VMT_OBJECT)) {
				struct mountpoint mp;
				mp.device = vmt2dataptr(mounts, VMT_OBJECT);
				mp.mountpoint = vmt2dataptr(mounts, VMT_STUB);
				output.push_back(mp);
			}
		}

		current = current + mounts->vmt_length;
	}
	free(buffer);

	return output;
}

std::vector<filesystem> stat_filesystems(std::vector<mountpoint> mounts) {
	std::vector<filesystem> output;

	for(auto it = mounts.begin(); it != mounts.end(); it++) {
		struct statvfs64 s;
		int rc = statvfs64((*it).mountpoint.c_str(), &s);

		if(rc < 0) {
			perror("Error getting statvfs64: ");
			exit(1);
		}

		struct filesystem fs;
		fs.mountpoint = (*it).mountpoint;
		fs.device = (*it).device;
		fs.avail_bytes = s.f_bavail * s.f_bsize;
		fs.size_bytes = s.f_blocks * s.f_bsize;
		fs.free_bytes = s.f_bfree * s.f_bsize;
		fs.files = s.f_files;
		fs.files_free = s.f_ffree;
		fs.files_avail = s.f_favail;

		output.push_back(fs);
	}

	return output;
}

#ifdef TESTING
int main() {
	auto fs = stat_filesystems(list_mounts());

	for(auto it = fs.begin(); it != fs.end(); it++) {
		std::cout << (*it).mountpoint << " " << (*it).size_bytes/1024 << " " << (*it).free_bytes/1024 << std::endl;
	}

	return 0;
}
#endif
