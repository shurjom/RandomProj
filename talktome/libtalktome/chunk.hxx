/*
 * chunk.hxx
 *
 *  Created on: May 31, 2015
 *      Author: Kristofer
 */

#ifndef CHUNK_HXX_
#define CHUNK_HXX_

#include "sentence.hxx"
#include <sstream>
#include <string>

class chunk {
public:
	chunk(long long offset = 0);
	void append(std::string article);
	bool tooBig();
	/**
	 * \brief
	 *   Number of sentences.
	 */
	unsigned int size() const { return sentences.size(); };
	/**
	 * \brief
	 *   Get the sentence at a location.
	 */
	const sentence& at(int i) const { return sentences[i]; };
	const sentence& at(const std::string& reference) const;
	string buildReference(int i) const { stringstream s; s << offset << " " << i; return s.str(); };
	string str() const { return data.str(); };
	/**
	 * \brief
	 *   Append the chunk to a stream and increment the offset accordingly.
	 */
	void write(int fd);
	void clear() {
		data.str("");
		sentences.clear();
	};
	/**
	 * \brief
	 *   Load the requested chunk from the given seekable stream.
	 */
	void read(int fd, std::string reference);
	virtual ~chunk();
private:
	std::stringstream data;
	long long offset;
	std::vector<sentence> sentences;
};

#endif /* CHUNK_HXX_ */
