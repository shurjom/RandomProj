/* PlayList.h declares class PlayList.
// * Student Name: Shurjo Maitra
// * Date: 2.15.16
// * Begun by: Joel Adams, for CS 112 at Calvin College.
// */

#ifndef PLAYLIST_H_
#define PLAYLIST_H_

#include "Song.h"
#include <vector>
#include <string>
using namespace std;

class PlayList {
public:
	PlayList(const string& fileName);
	unsigned getNumSongs() const;
	vector<Song> searchByArtist(const string& artist) const;
	vector<Song> searchByYear(const int year) const;
	vector<Song> searchByTitlePhrase(const string& phrase) const;
	void addSong(const Song& newSong);
	void removeSong(const Song& newSong);
	void save() const;
private:
	vector<Song> mySongs;
};

#endif /*PLAYLIST_H_*/
