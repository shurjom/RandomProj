/*
 * ttmbzfile.hxx
 *
 *  Created on: Jun 2, 2015
 *      Author: Kristofer
 */

#ifndef TTM_BZFILE_HXX_
#define TTM_BZFILE_HXX_

extern "C" {
#include <bzlib.h>
#include <stdio.h>
}

#include <vector>

class ttm_bzfile {
public:
	ttm_bzfile(int fd, const char *mode);
	ttm_bzfile(const char *path, const char *mode);
	int read(void *buf, int len);
	void read(std::vector<char>& buf);
	int write(void *buf, int len);
	virtual ~ttm_bzfile();
private:
	ttm_bzfile(FILE *file);
	FILE *file;
	BZFILE *bzfile;
	bool end_of_stream = false;
};

#endif /* TTM_BZFILE_HXX_ */
