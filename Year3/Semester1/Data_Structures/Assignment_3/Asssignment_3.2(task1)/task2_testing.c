#include <stdio.h>
//#include "bstdb.h"
//#include "bstdb.c"
int main() {
    bstdb_init();
    bstdb_add("Book1", 1000, "Author1");
    bstdb_add("Book2", 2000, "Author2");
    bstdb_add("Book3", 1500, "Author3");
    bstdb_add("Book4", 1800, "Author4");
    bstdb_stat(); // Call to print the statistics
    bstdb_quit();
    return 0;
}
