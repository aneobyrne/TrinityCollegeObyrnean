#ifndef ALBUM_H
#define ALBUM_H
#include <iostream>

using namespace std;

class Album {
    string artist;
    string title;
    int year;
    string displayAs;
public:
    Album(string newArtist, string newTitle, int newYear);
// getters and setters
    string GetArtist() const;
    string GetTitle() const;
    int GetYear() const;

    bool operator==(const Album &itemToCompare) const;
// returns true if album is "equal" to itemToCompare, i.e.,
// artist, title, year are all the same
    bool operator<(const Album &itemToCompare) const; //implemens sort order
// returns true if album is "smaller" than itemToCompare,i.e., // considers artist, title, and year in that order, i.e.,
// artist smaller - true
// artist larger - false
// artist equal - check title
// title smaller - true
// title larger - false
// title equal - check year
// year smaller - true
// otherwise - false
    operator const char *() const;
};
#endif //CONTACT_H
