#include <stdio.h>
#include "bst.h"
#include "bst.c"

int main() {
    char data[] = "FLOCCINAUCINIHILIPILIFICATION";  // Example data to create the tree
    Tree_Node* root = create_bst(data); //create the tree with given string data

    printf("Sorted Tree of given string \"%s\": \n", data);
    tree_print_sorted(root);
    printf("\n");

    char search_char;
    printf("Enter a character to search in the tree (type '.' to end): ");
    scanf(" %c", &search_char);
    while (search_char != '.') {
        Tree_Node* result = tree_search(root, search_char);
        if (result != NULL) 
          printf("Found '%c' in the tree!\n", result->data);
        else 
          printf("'%c' not found in the tree :(\n", search_char);

         printf("Enter another character to search (or type '.' to end): ");
         scanf(" %c", &search_char);
    }

    tree_delete(root); //clean memory by deleting tree
    return 0;
}
