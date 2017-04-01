#include "bzutil.hxx"

#include <vector>

extern "C" {
#include <bzlib.h>
}

void ttm_bzutil_compress(std::vector<char>& in, std::vector<char>& out)
{
	// SO says that &buf[0] has undefined behavior if the vector is empty, so ensure it isn't http://stackoverflow.com/a/1339767
	// BZ API says that to guarantee fit, resize to 101%+600 bytes. Since
	// in our application we're dealing with human text, just add some
	// extra bytes and we're probably going to be fine. We have the
	// buffer expansion logic in case.
	out.resize(in.size() + 1024);
	unsigned int destLen = out.size();
	char emptyBuf = '\0';
	while (BZ2_bzBuffToBuffCompress(
			&out[0],
			&destLen,
			in.size() ? &in[0] : &emptyBuf,
			in.size(),
			9,
			0,
			0) == BZ_OUTBUFF_FULL)
		out.resize(destLen = out.size() + 8192);
	// Trim bytes that aren't included.
	out.resize(destLen);
}
