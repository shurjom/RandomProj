#include <iostream>
#include <string>
using namespace std;

#include "chunk.hxx"
#include "sentence.hxx"

extern "C" {
#include <fcntl.h>
}

int main () {
	// Now take out all the words from this thingy
	cout << "This is doing something" << endl;
	string fakeChunk = "  And through 'them' days03 I was, \"a\" ghost atop my chair.";
	sentence theSentence(2, fakeChunk.size() - 2);
	cout << "chunk=" << fakeChunk << endl;
	cout << "sentence=" << theSentence.get(fakeChunk) << endl;
	auto words = theSentence.getWords(fakeChunk);
	cout << "the words are: " << endl;
	for (auto iter = words.begin(); iter != words.end(); iter++)
		cout << "\t" << *iter << endl;

	string anArticle = "In the beginning, God created the Heaven and the Earth. It was good, or so He said.";
	string anotherArticle = "In the beginning, the Word was with God. The Word was God. And things.";
	chunk theChunk;
	theChunk.append(anArticle);
	theChunk.append(anotherArticle);
	cout << "total sentences: " << theChunk.size() << endl;
	cout << "reference the second sentence with \"" << theChunk.buildReference(1) << "\"" << endl;
	cout << "the third sentence is \"" << theChunk.at(2).get(theChunk.str()) << "\"" << endl;

	// Stupid Windows... I thought it was just fopen(), but I was wrong -_-
#ifndef O_BINARY
#define O_BINARY
#endif
	const char *fname = "talkToMeText.dat";
	unlink(fname);
	int fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, S_IRUSR|S_IWUSR);
	cout << "opened as " << fd << endl;
	theChunk.write(fd);
	cout << "wrote" << endl;

	theChunk.append("This is short.");
	theChunk.append("My hand is tired out from all this article-writing.");
	// Manually search this chunk for a particular word.
	string aReference;
	string chunkStr(theChunk.str());
	for (unsigned int i = 0; i < theChunk.size(); i++)
	{
		auto sentenceWords = theChunk.at(i).getWords(chunkStr);
		cout << "got words" << endl;
		for (auto word_iter = sentenceWords.begin(); word_iter != sentenceWords.end(); word_iter++)
			if (*word_iter == "short")
			{
				aReference = theChunk.buildReference(i);
				cout << "Saving sentence with word \"" << *word_iter << "\" in it as reference \"" << aReference << "\"" << endl;
				break;
			}
	}
	auto chunk2_orig_data = theChunk.str();
	theChunk.write(fd);
	close(fd);

	// Now read in.
	fd = open("talkToMeText.dat", O_RDONLY|O_BINARY);
	chunk readerChunk;
	readerChunk.read(fd, aReference);
	cout << "The word is in sentence: \"" << readerChunk.at(aReference).get(readerChunk.str()) << "\"" << endl;

	// Check if what we read in has identical data to what we wrote out.
	cout << "matches? " << (readerChunk.str() == chunk2_orig_data) << " \"" << readerChunk.str().substr(0, 2) << "\" vs \"" << chunk2_orig_data.substr(0, 2) << "\"" << endl;

	aReference = "0 2";
	readerChunk.read(fd, aReference);
	cout << "The sentence at \"" << aReference << "\" is \"" << readerChunk.at(aReference).get(readerChunk.str()) << "\"" << endl;
	close(fd);
	return 0;
}

// Kristofer Brink is coolio
