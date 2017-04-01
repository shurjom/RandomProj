/*
 * chunk.cxx
 *
 *  Created on: May 31, 2015
 *      Author: Kristofer
 */

#include "bzutil.hxx"
#include "chunk.hxx"
#include "token.hxx"
#include "ttm_bzfile.hxx"

#include <algorithm>
#include <iostream>
#include <stdexcept>

extern "C" {
#include <unistd.h>
}

chunk::chunk(long long offset)
: offset(offset) {
	std::cerr << "Initialized chunk." << std::endl;
}

const sentence&
chunk::at(const std::string& reference) const
{
	std::stringstream reference_stream(reference);
	long long reference_offset;
	int reference_i;
	reference_stream >> reference_offset;
	if (reference_offset != offset)
		throw std::invalid_argument("reference refers to a different chunk.");
	reference_stream >> reference_i;
	return at(reference_i);
}

void chunk::append(std::string article) {
	std::stringstream article_header;
	article_header << article.size() << " ";

	// Get the sentences
	std::vector<pair<string, int>> sentence_strings;
	tokenize(article, sentence_strings, ".!?");
	for (auto iter = sentence_strings.begin(); iter != sentence_strings.end(); iter++) {
		sentences.push_back(sentence(((size_t)data.tellp()) + ((size_t)article_header.tellp()) + iter->second, iter->first.size()));
	}

	// Put into data. Header then data. <len> " " <data>.
	data << article_header.str() << article;
}

bool chunk::tooBig() {
	return data.tellp() > 2000000; // 2mbytes
}

void chunk::write(int fd) {
	std::cerr << "chunk::write()" << std::endl;
	auto databuf = data.str();
	// http://stackoverflow.com/a/2434208/2948122
	std::vector<char> in(databuf.begin(), databuf.end());
	std::vector<char> writeBuf;
	ttm_bzutil_compress(in, writeBuf);
	size_t writepos = 0;
	while (writepos < writeBuf.size())
	{
		std::cerr << "write()..." << std::endl;
		auto write_len = ::write(fd, &writeBuf[writepos], writeBuf.size() - writepos);
		std::cerr << "wrote " << write_len << " bytes" << std::endl;
		if (write_len == -1)
			throw std::runtime_error("problemz in hurr: unable to write chunk.");
		writepos += write_len;
	}
	offset += writepos;
	clear();
}

void chunk::read(int fd, std::string reference) {
	std::stringstream reference_stream(reference);
	reference_stream >> offset;

	off_t seek_step = 1;
	for (int i = sizeof(off_t) * 8 - 2; i > 0; i--)
		seek_step *= 2;
	// Unable to see if this haxx works until we actually
	// have a really large offset to search for...
	std::cerr << "using seek_step=" << seek_step << std::endl;

	long long current_offset = 0;
	auto seekret = lseek(fd, current_offset, SEEK_SET);
	std::cerr << "seek returns " << seekret << std::endl;
	while (current_offset < offset)
	{
		off_t next_seek = offset - current_offset;
		if (next_seek > seek_step)
			next_seek = seek_step;
		std::cerr << "seeking " << next_seek << std::endl;
		seekret = lseek(fd, next_seek, SEEK_CUR);
		std::cerr << "seek returns " << seekret << std::endl;
		current_offset += next_seek;
	}

	ttm_bzfile bzfile(fd, "rb");
	clear();
	std::vector<char> inbuf;
	bzfile.read(inbuf);
	// Need to change chunk data format so that we can read them in one at a time and just pass the data to append()
	// an article at a time. Without doing that, we'd introduce various problems.
	for (auto inbuf_iter = inbuf.begin(); inbuf_iter != inbuf.end();)
	{
		auto inbuf_space_location = std::find(inbuf_iter, inbuf.end(), ' ');
		if (inbuf_space_location == inbuf.end())
			throw std::logic_error("can't find no space");
		long long article_len;
		std::stringstream(std::string(inbuf_iter, inbuf_space_location)) >> article_len;
		auto article_begin = inbuf_space_location + 1;
		append(std::string(article_begin, article_begin + article_len));
		inbuf_iter = article_begin + article_len;
	}
}


chunk::~chunk() {
	// TODO Auto-generated destructor stub
}

