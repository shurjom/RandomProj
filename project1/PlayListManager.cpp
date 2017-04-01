/* tester.cpp tests the classes in our project.
// * Student Name: Shurjo Maitra
// * Date: 2.15.16
// * Begun by: Joel Adams, for CS 112 at Calvin College.
// */

#include "SongTester.h"
#include "PlayListTester.h"
#include "PlayList.h"
#include "Song.h"
#include <iostream>
using namespace std;

int main() {
    SongTester mt;
    mt.runTests();
    PlayListTester ct;
    ct.runTests();
    PlayList sc("testSongs.txt");
    while ( not 0 ) {
    	cout << "Welcome to the PlayList Manager!"<< "\n" << endl;
    	cout << "Please enter: " << endl;
    	cout << "1 - to search the PlayList for songs by a given artist" << endl;
    	cout << "2 - to search the PlayList for songs from a given year" << endl;
    	cout << "3 - to search the PlayList for songs with a given phrase in their title" << endl;
    	cout << "4 - to add a new song to the PlayList" << endl;
    	cout << "5 - to remove a song to the PlayList" << endl;
    	cout << "0 - to quit" << endl;
    	int choice;
    	cin >> choice;
    	if(choice == 1) {
    		string artist;
    		cout << "Please enter name of artist: " << endl;
    		cin >> artist;
    		vector<Song> v1 = sc.searchByArtist(artist);
    		for(unsigned i = 0; i < v1.size(); i++) {
    		cout << v1[i].getTitle() << '\n' << v1[i].getYear() << '\n' << endl;
    		}
    	} else if(choice == 2) {
    		int year;
    		cout << "Please enter the year: " << endl;
    		cin >> year;
    		vector<Song> v1 = sc.searchByYear(year);
    		for(unsigned i = 0; i < v1.size(); i++) {
    		cout << v1[i].getTitle() << '\n' << v1[i].getArtist()<< '\n' << endl;
    		}
    	} else if(choice == 3){
    		string phrase;
    		cout << "Please enter a phrase from the title of the Song: " << endl;
    		cin >> phrase;
    		vector<Song> v1 = sc.searchByTitlePhrase(phrase);
    		for(unsigned i = 0; i < v1.size(); i++) {
    		cout << v1[i].getTitle() << '\n' << v1[i].getYear()<< '\n' << endl;
    		}
    	} else if (choice == 4){
    		string title;
    		int year;
    		string artist;
    		cin.ignore(256, '\n');
    		cout << "Please enter title: " << endl;
    		cin >> title;
    		cout << "Please enter year: " << endl;
    		cin.ignore(256, '\n');
    		cin >> year;
    		cout << "Please enter name of the artist: " << endl;
    		cin >> artist;
    		cin.ignore(256, '\n');
    		sc.addSong(Song(title, artist, year));
    		int save;
    		cout << "Do you want to Save? Enter 9." << endl;
    		cin >> save;
    		if (save == 9) {
    			sc.save();
    			cout << "saved" << endl;
    		}
    	} else if (choice == 5) {
    		string title;
    		int year;
    		string artist;
    		cout << "Please enter title to remove: "<< endl;
    		cin.ignore(256, '\n');
    		cin >> title;
    		cout << "Please enter year to remove: " << endl;
    		cin.ignore(256, '\n');
    		cin >> year;
    		cout << "Please enter artist to remove: "<< endl;
    		cin.ignore(256, '\n');
    		sc.removeSong(Song(title, artist, year));
    		int save;
    		cout << "Do you want to Save? Enter 9." << endl;
    		cin >> save;
    		if (save == 9) {
    			sc.save();
    			cout << "saved" << endl;
    		}
    	} else if (choice == 0) {
    		cout << "End" << endl;
    		break;
    	}

    }
}



