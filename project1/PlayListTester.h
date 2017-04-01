/* PlayListTester.h tests the PlayList class.
// * Student Name: Shurjo Maitra
// * Date: 2.15.16
// * Begun by: Joel Adams, for CS 112 at Calvin College.
// */

#ifndef PLAYLISTTESTER_
#define PLAYLISTTESTER_

class PlayListTester {
public:
	void runTests();
	void testConstructors();
	void testSearchByArtist();
	void testSearchByYear();
	void testSearchByTitlePhrase();
	void testaddSongandremoveSong();
	void testSave() const;
};

#endif /*PLAYLISTTESTER_*/
