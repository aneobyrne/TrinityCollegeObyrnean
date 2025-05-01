#include <stdio.h> 
#include "t2.h"

int number_comparisons = 0;
int number_swaps = 0;


void insertionSort(int arr[], int size) { 
    for (int i = 1; i < size; i++) { //start at second element since for inserting we assume that at index 0 to be sorted
        int value = arr[i]; //value we want to sort
        int j = i - 1; //holds the value of previous index element
        
        while (j >= 0 && arr[j] > value) { //check if previous element (no negative) is greater than the value wanted to be sorted
            arr[j + 1] = arr[j]; //shift the previous element to the right
            j = j - 1; //continue moving to the next left (sorted) values until reaches the beginnig of the array
        }
        arr[j + 1] = value; //if loop fails value stays in its position, if not it moves to its sorted one
    }
  
}


void selectionSort(int arr[], int size) { //in which we will start selecting the smallest value
  for (int i = 0; i < size - 1; i++) { //searches for the smallest value in the array
        int min = i; 
        for (int j = i + 1; j < size; j++) { //compares values in the array with previous index value
            if (arr[j] < arr[min]) //if next value is smaller than previous minimum value
                min = j; //update minimum value index
        }
        if (min != i) //if minimum value changed
            swapElements(&arr[min], &arr[i]); //swap the updated minimum value with the first one
    }
} 


void quickSort(int arr[], int size) { 
    quickSorting(arr, 0, size - 1);
}

void swapElements(int* i, int* j) { //swaps two elements (positions)
  int h = *i; //temporary storing value
  *i = *j; 
  *j = h;  
}

int partitionQuicksort(int arr[], int smaller, int larger) { //sorts values in the array into smaller than and greater than pivot sections
  int pivot = arr[smaller]; //choose pivot to be the first element of the array
 // int pivot = arr[larger]; //choose pivot to be the last element of the array
 // int s = smaller;
 // int l = larger;
  int next = smaller + 1; //next value to the right of pivot

  for (int j = smaller + 1; j <= larger; j++) { //check the array values
        if (arr[j] < pivot) { //if current value is smaller than pivot we want to sort to the left
            swapElements(&arr[next], &arr[j]);
            next++; //continue to the next value in the array
        }
    }
    swapElements(&arr[smaller], &arr[next - 1]); //swap pivot to correct possition
    return next - 1;  //chosen pivot index
}

void quickSorting(int arr[], int smallest, int largest) {
    if (smallest < largest) { //until no more division is possible
        int pivot_index = partitionQuicksort(arr, smallest, largest); //sort array and get pivot index
        quickSorting(arr, smallest, pivot_index - 1); //continue sorting the left side of the pivot (smaller values)
        quickSorting(arr, pivot_index + 1, largest); //continue sorting the right side of the pivot (larger values)
    }
}

