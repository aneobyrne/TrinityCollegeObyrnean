#include "Album.h"
#include <iostream>

using namespace std;

Album::Album(string newArtist, string newTitle, int newYear){ //overloaded constructor
    this->artist = newArtist;
    this->title = newTitle;
    this->year = newYear;

    displayAs = artist + ", " + title + " (" + to_string(year) + ")";
}

string Album::GetArtist() const{
    return this->artist;
}

string Album::GetTitle() const{
    return this->title;
}

int Album::GetYear() const{
    return this->year;
}

bool Album::operator==(const Album &itemToCompare) const{ // returns true if album is "equal" to itemToCompare
    if ((artist == itemToCompare.artist) && (title == itemToCompare.title) && (year == itemToCompare.year))
        return true;
    else
        return false;
}

bool Album::operator<(const Album &itemToCompare) const{ // returns true if album is "smaller" than itemToCompare
    if (artist < itemToCompare.artist)
        return true;
    else if (artist > itemToCompare.artist)
        return false;
    else {
        if (title < itemToCompare.title)
            return true;
        else if (title > itemToCompare.title)
            return false;
        else {
            if (year < itemToCompare.year)
                return true;
            else if (year > itemToCompare.year)
                return false;
            else
                return false;
        }
    }
}

Album::operator const char *() const{ //returns a pointer to the underlying character array
    return displayAs.c_str();
}
