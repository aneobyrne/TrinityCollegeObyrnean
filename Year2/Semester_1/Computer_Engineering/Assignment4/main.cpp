#include "AlbumCollection.h"
using namespace std;
AlbumCollection db;
void InsertAlbums(){
    db.InsertAlbum("The Beatles", "Sgt Pepper's Lonely Hearts Club Band", 1967);
    db.InsertAlbum("Jay-Z", "The Blueprint", 2001);
    db.InsertAlbum("The White Stripes", "Elephant", 2003);
    db.InsertAlbum("Dua Lipa", "Future Nostalgia", 2020);
    db.InsertAlbum("Taylor Swift", "Red (Taylor's Version)", 2021);
    db.InsertAlbum("The Beatles", "Yellow Submarine", 1969);
    db.InsertAlbum("The Plagiarist", "Future Nostalgia", 2021);
}
int main() {
    InsertAlbums();
    db.PrintByYear(2021);
}
