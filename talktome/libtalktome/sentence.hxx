/*
 * sentence.hxx
 *
 *  Created on: May 31, 2015
 *      Author: Kristofer
 */

#ifndef SENTENCE_HXX_
#define SENTENCE_HXX_

#include <vector>
#include <string>
using namespace std;

// A description of a particular sentence within a chunk.
class sentence {
public:
	sentence(int offset, int len);
	/**
	 * \brief
	 *   Extract the string from a chunk.
	 */
	string get(const string&chunk) const;
	vector<string> getWords(const string& chunk) const;
	virtual ~sentence();
private:
	int offset;
	int len;
};

#endif /* SENTENCE_HXX_ */
