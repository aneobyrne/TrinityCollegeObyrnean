#include "AlbumCollection.h"
#include <iostream>
#include <set>
#include <string>

using namespace std;

void AlbumCollection::InsertAlbum(string artist, string title, int year){ // inserts an album with given attributes
    setAlbums.insert(Album(artist, title, year));
}

void AlbumCollection::Print(){ // prints list of all albums
   for (Album album : setAlbums)
       cout << album << endl;
}

void AlbumCollection::PrintByTitleSubstring(string titleSubstring){ // prints list of all albums where title contains <titleSubstring>
    for (Album album : setAlbums){
        auto titleFound = album.GetTitle().find(titleSubstring);
        if (titleFound != string::npos)
            cout << album << endl;
    }
}

void AlbumCollection::PrintByArtist(string artist){ // prints list of all albums where artist = <artist>
    for (Album album : setAlbums){
        if (album.GetArtist() == artist)
            cout << album << endl;
    }
}

void AlbumCollection::PrintByYear(int year){ // prints list of all albums where year = <year>
    for (Album album : setAlbums){
        if (album.GetYear() == year)
            cout << album << endl;
    }
}

bool AlbumCollection:: ContainsTitle(string title){ // returns true if collection contains an album where title = <title>
    for (Album album : setAlbums){
        if (album.GetTitle() == title)
            return true;
    }
    return false;
}

bool  AlbumCollection::ContainsArtist(string artist){ // returns true if collection contains an album where artist = <artist>
    for (Album album : setAlbums){
        if (album.GetArtist() == artist)
            return true;
    }
    return false;
}

bool AlbumCollection::ContainsYear(int year){ // returns true if collection contains an album where year = <year>
    for (Album album : setAlbums){
        if (album.GetYear() == year)
            return true;
    }
    return false;
}

void AlbumCollection::DeleteAlbumByTitle(string titleToSearch){ // removes the FIRST album where title = <title> from the collection
   bool exists= false;
    for (Album album : setAlbums){
        if (album.GetTitle() == titleToSearch) {
            setAlbums.erase(album);
            exists = true;
            break;
        }
    }
    if (!exists)
        cout << "No album found!" << endl;
}

void AlbumCollection::DeleteAlbumsByArtist(string artistToSearch){ // removes ALL albums from collection where artist = <artist>
    int counter= 0;
    for (Album album : setAlbums){
        if (album.GetArtist() == artistToSearch) {
            setAlbums.erase(album);
            counter++;
        }
    }
    if (counter == 0)
        cout << "No album found!" << endl;
}

int AlbumCollection::GetNumberAlbums(){ // returns the number of albums stored in the collection
    int counter = 0;
    for (Album album : setAlbums)
        counter++;
    return counter;
}
