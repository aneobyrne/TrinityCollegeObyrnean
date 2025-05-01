#include <stdio.h>
#include <stdlib.h>
#include "t1.h"


//Fills the array with ascending, consecutive numbers, starting from 0.
void fill_ascending(int *array, int size) {
 for (int i = 0; i < size; i++) {
        array[i] = i; //fills with ascending numbers from 0 to the given size -1 in order
 } 
}
//Fills the array with descending numbers, starting from size -1
void fill_descending(int *array, int size) {
 for (int i = 0; i < size; i++) {
        array[i] = size - 1 - i; //fills with descending numbers from the given size -1 to 0 in order
 }
}

//Fills the array with uniform numbers.
void fill_uniform(int *array, int size){
 int value_of_choice = 3; //you can choose any value to want to be repeated
    for (int i = 0; i < size; i++) {
        array[i] = value_of_choice; //always fill array slots with the chosen value
    }
    
}

//Fills the array with random numbers within 0 and size-1. Duplicates are allowed.
void fill_with_duplicates(int *array, int size){
for (int i = 0; i < size; i++) {
        array[i] = rand() % size; //provides random numbers between 0 and size-1 (duplicates are nto taken into account)
    }
    
}


//Fills the array with unique numbers between 0 and size-1 in a shuffled order. Duplicates are not allowed.
void fill_without_duplicates(int *array, int size){
    for (int i = 0; i < size; i++) {
        array[i] = i; //array with no duplicate numbers
    }
    
    for (int i = size - 1; i > 0; i--) { //shuffle array randomly
        int j = rand() % (i + 1);
        //swap array[i] with random order position for random but unique elements 
        int num = array[i];
        array[i] = array[j];
        array[j] = num;
    }
    
}

void printArray(int* arr, int size){
  int i;
  for(i=0; i<size;i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}
