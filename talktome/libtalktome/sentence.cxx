/*
 * sentence.cxx
 *
 *  Created on: May 31, 2015
 *      Author: Kristofer
 */

#include "sentence.hxx"
#include "token.hxx"
#include <algorithm>
#include <cctype>

sentence::sentence(int offset, int len)
: offset(offset)
, len(len) {
}

string sentence::get(const string& chunk) const {
	return chunk.substr(offset, len);
}



static void dePunctuate(std::vector<string> &words) {
	for (auto iter = words.begin(); iter != words.end(); iter++) {

		// Steve314 http://stackoverflow.com/a/19139765/2948122
		iter->erase (std::remove_if (iter->begin (), iter->end (), ptr_fun<int, int>(ispunct)), iter->end ());
	}
}

static void lowerCase(std::vector<string> &words) {
	for (auto iter = words.begin(); iter != words.end(); iter++) {
		// Stefan Mai  http://stackoverflow.com/a/313990/2948122
		std::transform(iter->begin(), iter->end(), iter->begin(), ::tolower);
	}
}

vector<string> sentence::getWords(const string& chunk) const {
	vector<pair<string, int>> words;
	tokenize(get(chunk), words, ", -=+\t\n\r");
	vector<string> plain_words;
	for (auto iter = words.begin(); iter != words.end(); iter++)
		plain_words.push_back(iter->first);
	dePunctuate(plain_words);
	lowerCase(plain_words);
	return plain_words;
}

sentence::~sentence() {
}

