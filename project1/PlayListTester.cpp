/* PlayListTester.cpp defines the PlayList test-methods.
// * Student Name: Shurjo Maitra
// * Date: 2.15.16
// * Begun by: Joel Adams, for CS 112 at Calvin College.
// */

#include "PlayListTester.h"
#include <iostream>
#include "PlayList.h"
#include <cassert>
using namespace std;

void PlayListTester::runTests() {
	cout << "\nTesting class PlayList..." << endl;
	testConstructors();
	testSearchByArtist();
	testSearchByYear();
	testSearchByTitlePhrase();
	testaddSongandremoveSong();
	testSave();
	cout << "All tests passed!" << endl;
}

void PlayListTester::testConstructors() {
	cout << "- constructors..." << flush;
	PlayList pList("testSongs.txt");
	assert(pList.getNumSongs() == 4);
	cout << " 0 " << flush;

	cout << " Passed!" << endl;
}

void PlayListTester::testSearchByArtist() {
	cout << "- searchByArtist()... " << flush;
	// load a playlist with test songs
	PlayList pList("testSongs.txt");

	// empty case (0)
	vector<Song> searchResult = pList.searchByArtist("Cream");
	assert(searchResult.size() == 0);
	cout << " 0 " << flush;

	// case of 1
	searchResult = pList.searchByArtist("Baez");
	assert(searchResult.size() == 1);
	assert(searchResult[0].getTitle() == "Let It Be");
	cout << " 1 " << flush;

	// case of 2
	searchResult = pList.searchByArtist("Beatles");
	assert(searchResult.size() == 2);
	assert(searchResult[0].getTitle() == "Let It Be");
	assert(searchResult[1].getTitle() == "Penny Lane");
	cout << " 2 " << flush;

	cout << " Passed!" << endl;
}

void PlayListTester::testSearchByYear() {
	cout << "- searchByYear()... " << flush;
	// load a playlist with test songs
	PlayList pList("testSongs.txt");

	// empty case (0)
	vector<Song> searchResult = pList.searchByYear(2012);
	assert(searchResult.size() == 1);
	cout << " 0 " << flush;

	// case of 1
	searchResult = pList.searchByYear(1971);
	assert(searchResult.size() == 1);
	assert(searchResult[0].getTitle() == "Let It Be");
	cout << " 1 " << flush;

	// case of 2
	searchResult = pList.searchByYear(1967);
	assert(searchResult.size() == 2);
	assert(searchResult[0].getTitle() == "Let It Be");
	assert(searchResult[1].getTitle() == "Penny Lane");
	cout << " 2 " << flush;

	cout << " Passed!" << endl;
}

void PlayListTester::testSearchByTitlePhrase() {
	cout << "- searchByTitlePhrase()... " << flush;
	// load a playlist with test songs
	PlayList pList("testSongs.txt");

	// empty case (0)
	vector<Song> searchResult = pList.searchByTitlePhrase("Cream");
	assert(searchResult.size() == 0);
	cout << " 0 " << flush;

	// case of 1
	searchResult = pList.searchByTitlePhrase("Let");
	assert(searchResult.size() == 2);
	assert(searchResult[0].getTitle() == "Let It Be");
	cout << " 1 " << flush;

	// case of 2
	searchResult = pList.searchByTitlePhrase("Penny");
	assert(searchResult.size() == 1);
	assert(searchResult[0].getTitle() == "Penny Lane");
	cout << " 2 " << flush;
	cout << " Passed!" << endl;
}

void PlayListTester::testaddSongandremoveSong() {
	cout << "- addSong()... " << flush;
	// load a playlist with test songs
	PlayList pList("testSongs.txt");

	//add a song and check
	Song newSong("One", "Metallica", 1969);
	assert(newSong.getTitle() == "One");
	cout << " 0 " << flush;

	pList.addSong(newSong);
	vector<Song> searchResult = pList.searchByArtist("Metallica");
	assert(searchResult[0].getTitle() == "One");
	cout << " 1 " << flush;

	Song toremove("One", "Metallica", 1969);
	pList.removeSong(toremove);
	cout << " 2 " << flush;

	vector<Song> searchResult2 = pList.searchByArtist("Metallica");
	assert(searchResult2.size() == 0);
	cout << " 3 " << flush;

	cout << "Passed!" << endl;

}

void PlayListTester::testSave() const {
	cout << "- Save()..." << flush;

	// load a playlist that is to be changed
	PlayList pList("testSongs.txt");

	//add a song to cause a change in the original playlist
	Song s1("Trade Mistakes", "Panic! At the Disco", 2011);
	pList.addSong(s1);

	// write the modified playlist back into the original file
	pList.save();
	cout << " 0 " << flush;

	// load the saved file under a new playlist name and test for the added song
	PlayList pList2("testSongs.txt");
	vector<Song> searchResult = pList2.searchByArtist("Panic! At the Disco");
	assert(searchResult.size() == 1);
	cout << " 1 " << flush;

	//return the playlist to its original state
	pList.removeSong(s1);
	pList.save();
	cout << " 2 " << flush;

	// check that the song has been removed
	vector<Song> searchResult2 = pList2.searchByArtist("Panic! At the Disco");
	assert(searchResult2.size() == 1);
	cout << " 3 " << flush;
	cout << " Passed!" << endl;

}
