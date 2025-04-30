#ifndef ALBUMCOLLECTION_H
#define ALBUMCOLLECTION_H
#include "Album.h"
#include <set>
class AlbumCollection {
    set<Album> setAlbums;
// set of albums in the collection
//
// It is not intended that the same album is stored multiple times.
// If someone tries that anyway, the behaviour of AlbumCollection
// is undefined. (We will not have such cases in our tests.)
public:
    void InsertAlbum(string artist, string title, int year);
// inserts an album with given attributes
    void Print();
// prints list of all albums
    void PrintByTitleSubstring(string titleSubstring);
// prints list of all albums where title contains <titleSubstring>
    void PrintByArtist(string artist);
// prints list of all albums where artist = <artist>
    void PrintByYear(int year);
// prints list of all albums where year = <year>
    bool ContainsTitle(string title);
// returns true if collection contains an album where title = <title>
    bool ContainsArtist(string artist);
// returns true if collection contains an album where artist = <artist>
    bool ContainsYear(int year);
// returns true if collection contains an album where year = <year>
    void DeleteAlbumByTitle(string titleToSearch);
// removes the FIRST album where title = <title> from the collection
// if no such album is found, prints "No album found!" (without the quotes) to standard output.
    void DeleteAlbumsByArtist(string artistToSearch);
// removes ALL albums from collection where artist = <artist>
// if no such album is found, prints "No album found!" (without the quotes) to standard output.
    int GetNumberAlbums();
// returns the number of albums stored in the collection
};
#endif // ALBUMCOLLECTION_H
