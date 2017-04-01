/* PlayList.cpp defines the PlayList methods.
// * Student Name: Shurjo Maitra
// * Date: 2.15.16
// * Begun by: Joel Adams, for CS 112 at Calvin College.
// */
#include "PlayList.h"
#include <fstream>      // ifstream
#include <cassert>      // assert()
#include <vector>
using namespace std;
/* PlayList constructor
 * @param: fileName, a string
 * Precondition: fileName contains the name of a playlist file.
 */
PlayList::PlayList(const string& fileName) {
	// open a stream to the playlist file
	ifstream fin(fileName.c_str());
	assert(fin.is_open());

	// read each song and append it to mySongs
	Song s;
	while (true) {
		s.readFrom(fin);
		if (!fin) {
			break;
		}
		string separator;
		getline(fin, separator);
		mySongs.push_back(s);
	}

	// close the stream
	fin.close();
}

/* Retrieve length of the playList
 * Return: the number of songs in the playList.
 */
unsigned PlayList::getNumSongs() const {
	return mySongs.size();
}

vector<Song> PlayList::searchByArtist(const string& directorName) const {
	/* searchByArtist() searches the PlayList for Songs by a given artist.
	    *
	    * @param: artistName, a string containing the artist's name (or a part of it)
	    * @return: a vector containing all Songs in the PlayList
	    *           for whom artist is a substring of myArtist.
	    */
   vector<Song> v;
   for (unsigned i = 0; i < mySongs.size(); i++) {
      if ( mySongs[i].getArtist().find(directorName) != string::npos ) {
         v.push_back( mySongs[i] );
      }
   }
   return v;
}

vector<Song> PlayList::searchByYear(int year) const {
	/* searchByYear() searches the PlayList for Songs by a given year.
	    *
	    * @param: year, a integer containing the song year
	    * @return: a vector containing all songs in the playList
	    *           for whom year is a substring of myYear.
	    */
	vector<Song> v;
	for (unsigned i = 0; i < mySongs.size(); i++){
		if ( mySongs[i].getYear() == year ) {
			v.push_back( mySongs[i] );
		}
	}
	return v;
}

vector<Song> PlayList::searchByTitlePhrase(const string& phrase) const {
	vector<Song> v;
	/* searchByTitlePhrase() searches the PlayList for Songs by a given title phrase.
	    *
	    * @param: phrase, a string within a title of the song
	    * @return: a vector containing all Songs in the playList
	    *           for whom title phrase is a substring of myTitle.
	    */
	 for (unsigned i = 0; i < mySongs.size(); i++) {
	      if ( mySongs[i].getTitle().find(phrase) != string::npos ) {
	         v.push_back( mySongs[i] );
	      }
	   }
	   return v;
	}

void PlayList::addSong(const Song& newSong) {
	/* addSong() adds a new song in the playList for Songs by asking for user input.
	    *
	    * @param: newSong, a string containing a separate Song object created through user input.
	    * @return: this does not return anything but appends the newSong class in the memory.
	    */
		mySongs.push_back( newSong );
}

void PlayList::removeSong(const Song& aSong) {
	//	/* removeSong() removes a song from the PlayList of movies through user input information.
	//	    *
	//	    * @param: aSong, a string containing an already Song object found through user input information.
	//	    * @return: this does not return a vector my removes a Song object from the PlayList of movies.
	//	    */
		vector<Song>::iterator i = mySongs.begin();
		while ( i != mySongs.end()) {
			if ( i -> getTitle() == aSong.getTitle()) {
				i = mySongs.erase(i);
			}
			else {
				++i;
			}
		}
}

void PlayList::save() const {
	/* save() saves and writes the Song object created through user input (newSong) into the PlayList for Songs.
	    *
	    * @param: this method has no parameters
	    * @return: this method does not return but writes the appended Songs objected into the testSongs.txt file.
	    */
	ofstream fout("testSongs.txt");
	for (unsigned i = 0; i < mySongs.size(); i++) {
		mySongs[i].writeTo (fout);
		fout << '\n';
	}
	fout.close();
}
