#include "t4.h"
#include <stdio.h> 
#include <stdlib.h>
#include "task4top5.c"
//#include "task4top10.c"

void printAllGames();

int main(){ 

    if (load_file("t4_ignshortened.csv") == 0)
        printf("Error with loading file");
    else 
        printGames();
     // printAllGames();

    return 0; 
}

/*void printAllGames() {
    printf("All games and their scores:\n");
    for (int j = 0; j < totalGames; j++) {
        printf("Title: %s, Platform: %s, Score: %s, Release Year: %s\n", 
               allGames[j].title, 
               allGames[j].platform, 
               allGames[j].score, 
               allGames[j].release_year);
    }
    printf("\n");
} */




