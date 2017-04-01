/* Song.cpp defines the methods for class Song (see Song.h).
// * Student Name: Shurjo Maitra
// * Date: 2.15.16
// * Begun by: Joel Adams, for CS 112 at Calvin College.
// */

#include "Song.h"
#include <cstdlib>
#include <fstream>

/* Song default constructor
 * Postcondition: myTitle and myArtist are empty strings
 *            && myYear == 0.
 */
Song::Song() {
	myTitle = myArtist = "";
	myYear = 0;
}

/* getter method for myTitle
 * Return: myTitle
 */
string Song::getTitle() const {
	return myTitle;
}

/* getter method for myArtist
 * Return: myArtist
 */
string Song::getArtist() const {
	return myArtist;
}

/* getter method for myYear
 * Return: myYear
 */
unsigned Song::getYear() const {
	return myYear;
}

/* Explicit constructor
 * @param: title, a string
 * @param: artist, a string
 * @year: an unsigned int.
 * Postcondition: myTitle == title &&
 *                myArtist == artist &&
 *                myYear == year.
 */
Song::Song(const string& title, const string& artist, unsigned year) {
	myTitle = title;
	myArtist = artist;
	myYear = year;
}

/* Song input method...
 * @param: in, an istream
 * Precondition: in contains the title, artist, and year data for a Song.
 * Postcondition: the title, artist, and year data have been read from in &&
 *                 myTitle == title &&
 *                 myArtist == artist &&
 *                 myYear == year.
 */
void Song::readFrom(istream& in) {
	getline(in, myTitle);
	getline(in, myArtist);
	string yearString;
	getline(in, yearString);
	myYear = atoi(yearString.c_str());
}

/* Song output...
 * @param: out, an ostream
 * Postcondition: out contains myTitle, a newline,
 *                             myArtist, a newline,
 *                             myYear, and a newline.
 */
void Song::writeTo(ostream& out) const {
	out << myTitle << '\n'
	    << myArtist << '\n'
	    << myYear  << '\n';
}

bool Song::operator==(const Song& Song2) const {
	if (myTitle != Song2.getTitle()){
		return false;
	}
	else if(myYear != Song2.getYear()){
		return false;
	}
	else if(myArtist != Song2.getArtist()) {
		return false;
	}
	else {
		return true;
	}
}


