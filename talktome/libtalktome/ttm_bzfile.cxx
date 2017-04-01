/*
 * ttmbzfile.cxx
 *
 *  Created on: Jun 2, 2015
 *      Author: Kristofer
 */

#include "ttm_bzfile.hxx"

extern "C" {
#include <stdio.h>
#include <unistd.h>

#ifdef WIN32
extern FILE *fdopen(int, const char *);
#endif
}

ttm_bzfile::ttm_bzfile(FILE *file)
: file(file)
, bzfile(NULL)
{
	int bzerr = BZ_OK;
	bzfile = BZ2_bzReadOpen(&bzerr, file, 0, 0, NULL, 0);
}

ttm_bzfile::ttm_bzfile(int fd, const char *mode)
: ttm_bzfile(fdopen(dup(fd), mode)) {
}

ttm_bzfile::ttm_bzfile(const char *path, const char *mode)
: ttm_bzfile(fopen(path, mode)) {
}

int
ttm_bzfile::read(void *buf, int len) {
	if (end_of_stream)
		return 0;
	int bzerr = 0;
	len = BZ2_bzRead(&bzerr, bzfile, buf, len);
	if (bzerr == BZ_STREAM_END)
		end_of_stream = true;
	else if (bzerr != BZ_OK)
		return -1;
	return len;
}

void ttm_bzfile::read(std::vector<char> &buf) {
	size_t pos = 0;
	auto chunk_size = 8192;
	int read_len = 0;
	do
	{
		pos += read_len;
		if (buf.size() < pos + chunk_size)
			buf.resize(pos + chunk_size);
		read_len = read(&buf[pos], buf.size() - pos);
	} while (read_len > 0);
	buf.resize(pos);
}

int
ttm_bzfile::write(void *buf, int len) {
	return BZ2_bzwrite(bzfile, buf, len);
}

ttm_bzfile::~ttm_bzfile() {
	BZ2_bzclose(bzfile);
	fclose(file);
}
