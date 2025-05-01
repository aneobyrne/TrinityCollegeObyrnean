#include "t1.h"
#include <stdio.h>
#include "task1.c"



int main()
{
    int size = 6;
    int arr[6];

    printf("Ascending Array: \n");
    fill_ascending(arr, size);
    printArray(arr, size);

    printf("Descending Array: \n");
    fill_descending(arr, size);
    printArray(arr, size);

    printf("Single value Array (3): \n");
    fill_uniform(arr, size);
    printArray(arr, size);

    printf("Random Array (with duplicates): \n");
    fill_with_duplicates(arr, size);
    printArray(arr, size);

    printf("Random Array (without duplicates): \n");
    fill_without_duplicates(arr, size);
    printArray(arr, size);

    return 0;

}
