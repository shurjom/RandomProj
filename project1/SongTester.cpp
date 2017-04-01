/* SongTester.cpp defines the test-methods for class SongTester.
// * Student Name: Shurjo Maitra
// * Date: 2.15.16
// * Begun by: Joel Adams, for CS 112 at Calvin College.
// */

#include "SongTester.h"
#include <iostream>
#include <cassert>
#include "Song.h"
#include <fstream>
using namespace std;

void SongTester::runTests() {
	cout << "Testing class Song..." << endl;
	testConstructors();
	testReadFrom();
	testWriteTo();
	testOperator();
	cout << "All tests passed!" << endl;
}

void SongTester::testConstructors() {
	cout << "- constructors ... " << flush;
	// default constructor
	Song s;
	assert(s.getTitle() == "");
	assert(s.getArtist() == "");
	assert(s.getYear() == 0);
	cout << " 0 " << flush;
	// explicit constructor
	Song s1("Badge", "Cream", 1969);
	assert(s1.getTitle() == "Badge");
	assert(s1.getArtist() == "Cream");
	assert(s1.getYear() == 1969);
	cout << " 1 " << flush;

	cout << " Passed!" << endl;
}

void SongTester::testReadFrom() {
	cout << "- readFrom()... " << flush;
	ifstream fin("testSongs.txt");
	assert(fin.is_open());
	Song s;

	// read first song in test playlist
	s.readFrom(fin);
	assert(s.getTitle() == "Call Me Maybe");
	assert(s.getArtist() == "Carly Rae Jepsen");
	assert(s.getYear() == 2012);
	cout << " 0 " << flush;
	string separator;
	getline(fin, separator);

	// read second song in test playlist
	s.readFrom(fin);
	assert(s.getTitle() == "Let It Be");
	assert(s.getArtist() == "The Beatles");
	assert(s.getYear() == 1967);
	cout << " 1 " << flush;
	getline(fin, separator);

	// read third song in test playlist
	s.readFrom(fin);
	assert(s.getTitle() == "Let It Be");
	assert(s.getArtist() == "Joan Baez");
	assert(s.getYear() == 1971);
	cout << " 2 " << flush;

	cout << "Passed!" << endl;
}

void SongTester::testWriteTo() {
	cout << "- writeTo()... " << flush;

	// declare three songs
	Song s1("Badge", "Cream", 1969);
	Song s2("Godzilla", "Blue Oyster Cult", 1977);
	Song s3("Behind Blue Eyes", "The Who", 1971);

	// write the three songs to an output file
	ofstream fout("../testSongOutput.txt");
	assert(fout.is_open());
	s1.writeTo(fout);
	s2.writeTo(fout);
	s3.writeTo(fout);
	fout.close();

	// use readFrom() to see if writeTo() worked
	ifstream fin("../testSongOutput.txt");
	assert(fin.is_open());
	Song s4, s5, s6;

	// read and check the first song
	s4.readFrom(fin);
	assert(s4.getTitle() == "Badge");
	assert(s4.getArtist() == "Cream");
	assert(s4.getYear() == 1969);
	cout << " 0 " << flush;

	// read and check the second song
	s5.readFrom(fin);
	assert(s5.getTitle() == "Godzilla");
	assert(s5.getArtist() == "Blue Oyster Cult");
	assert(s5.getYear() == 1977);
	cout << " 1 " << flush;

	// read and check the third song
	s6.readFrom(fin);
	assert(s6.getTitle() == "Behind Blue Eyes");
	assert(s6.getArtist() == "The Who");
	assert(s6.getYear() == 1971);
	cout << " 2 " << flush;

	fin.close();
	cout << " Passed!" << endl;
}

void SongTester::testOperator() {
	cout << "- operator()... " << flush;

	// declare three songs
	Song s1("One", "Metallica", 1969);
	Song s2("Bat Country", "Avenged Sevenfold", 1977);
	Song s3("Sunburn", "Muse", 1971);
	assert(s1.getTitle() == "One");
	assert(s2.getTitle() == "Bat Country");
	assert(s3.getTitle() == "Sunburn");

	//if the 1st and 2nd same
	if (s1.operator== (s2) == true) {
		cout << "1st and 2nd songs are same" << endl;
	} else {
		cout << "1st and 2nd songs are different" << endl;
	}
	cout << " 0 " << flush;

	//if the 2nd and 3rd songs are same
	if (s2.operator == (s3) == true) {
		cout << "2nd and 3rd songs are same" << endl;
	} else {
		cout << "2nd and 3rd songs are different" << endl;
	}
	cout << " 1 " << flush;

	cout << " Passed!" << endl;
}
