#include "bstdb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// The input data will be books. A book is comprised of a title and a word
// count. You should store both these values in the tree along with a unique
// integer ID which you will generate.
int Max(int a, int b) {  //get max
    return (a > b) ? a : b; 
}
//global declarations (used throught the whole program)
BookNode *root = NULL;
int next_id = 1;
int insertion_count = 0;

int bstdb_init ( void ) { //initializes database
    root = NULL; //empty root of tree
    next_id = 1; //reset ID counter
    srand(time(NULL));
    return 1;
}

BookNode *create_node(int id, char *name, int word_count, char *author) { //creates book node in the tree
    BookNode *new_node = (BookNode *)malloc(sizeof(BookNode));
    if (!new_node) {
        return NULL;
    }
    //provide book data:
    new_node->id = id;
    new_node->name = strdup(name); //copy function
    new_node->word_count = word_count;
    new_node->author = strdup(author);
    new_node->left = new_node->right = NULL; //its a leaf node for now
    new_node->level = 1;
    return new_node;
}
int bstdb_add ( char *name, int word_count, char *author ) { //creates new node in the BST, with the book name, word count and author
 int doc_id = get_unique_id(); //next ID is an incremented unique ID
    BookNode *new_node = create_node(doc_id, name, word_count, author); //creates new node for book and its data
    if (!new_node) //if no node was created
        return -1;
    if (!root) 
        root = new_node; //if tree is empty set as root
     else { //if tree not empty
        BookNode *current = root; //start at the starting node (root)
        BookNode *parent = NULL; //has no highehr node
        while (current) { //until finds a NULL node
            parent = current; //move to next node
            if (doc_id < current->id) //if the ID given to the added book is smaller than the current node boook ID
                current = current->left; //we go left for smaller values
             else 
                current = current->right; //we go right for larger values
        }
        if (doc_id < parent->id) //if the ID given to the added book is smaller thant the parents ID
            parent->left = new_node; //place the node to the left since smaller
         else 
            parent->right = new_node; //place the node to the right since larger
    }
    root = balance_tree(root); //balance tree fofr every insertion
    insertion_count++; //increment count for statistics
    return doc_id;
}
int bstdb_get_word_count ( int doc_id ) { //search an ID and return its word count if found
  BookNode *current = root; //start at root
    while (current) { //while its not NULLL
        if (doc_id == current->id)  //if we find an ID equal to the one we're searching
            return current->word_count; //return the word count of the book
         else if (doc_id < current->id) //else, continue searching throught tree
            current = current->left;
         else 
            current = current->right;
    }
    return -1; //not found
}
char* bstdb_get_name ( int doc_id ) { //search an ID and return its book name if found
 BookNode *current = root; //startign at root
    while (current) { //while its not NULL
        if (doc_id == current->id) //search for the ID
            return current->name; //if found return the name of the book with that ID
         else if (doc_id < current->id) //else continue searching thorught the tree
            current = current->left;
         else 
            current = current->right;
    }
    return 0; //not found
}
void bstdb_stat ( void ) { //prints out statistics of the tree
    printf("STAT\n");
    printf("Is tree balanced? -> ");
    int is_balanced = check_balance(root); //1. Check if BST is balanced
    if (is_balanced) 
        printf("Tree is balanced\n");
     else 
        printf("Tree is not balanced\n"); 
    int node_count = count_nodes(root); //2. Total number of nodes and if matches insertion count
    printf("Total number of nodes in the tree: -> %d\n", node_count);
    printf("Total number of insertions performed: -> %d\n", insertion_count);
    if (node_count == insertion_count) 
        printf("Node count matches insertion count.\n");
     else 
        printf("Mismatch in node count and insertion count.\n");
    float avg_traversal = 0.0;
    if (node_count > 0)
        avg_traversal = calculate_avg_traversal(root); //3. Calculate average node traversal
    printf("Average traversal of nodes: -> %.2f\n", avg_traversal);
    int *visited_ids = malloc(sizeof(int) * node_count);
    int unique_ids = check_unique_ids(root, visited_ids, 0); //4. Check all ID's to prove uniqueness
    free(visited_ids);
      if (unique_ids) 
        printf("All document IDs are unique\n");
     else 
        printf("All document IDs are not unique\n");
}
void bstdb_quit ( void ) { //free the BST and all its nodes
    tree_delete(root); //delete left subtree
    root = NULL; 
}
void tree_delete(BookNode* root){
    if (root == NULL) 
       return; //nothing to do if empty (end reached)
    tree_delete(root->left); //delete left subtree
    tree_delete(root->right); //delete right subtree
    free(root->name);
    free(root->author);
    free(root);
}

//GET RANDOM UNIQUE IDs:
int check_uniqueness(int id, BookNode *node) {
    if (!node) 
    return 1;
    if (node->id == id)
    return 0;
    return check_uniqueness(id, node->left) && check_uniqueness(id, node->right);
}

int get_unique_id(void) {
    int id;
    while(!check_uniqueness(id, root)){
        id =rand();
    }
    return id;
}

//TREE BALANCING EXTRA FUNCTIONS (BASED ON AVL):
int level(BookNode *node) { //gets in which level of the tree the node is (how long th esubtree is)
   if (node == NULL)
        return 0;
    return node->level; //return level of node
}
int get_balance_factor(BookNode *node) { //difference between right and left subtrees
    if (node == NULL)
        return 0;
    return level(node->left) - level(node->right); //compare both subtrees to check how balanced (we want between -1 and 1)
}
BookNode* balance_tree(BookNode* node) { //balance the tree
    if (!node) 
     return node;
    int balance_factor = get_balance_factor(node);
    if (balance_factor > 1) { //left too long
        if (get_balance_factor(node->left) >= 0) //if left child of the node is also too long or balanced
            return right_rotate(node); //perform right rotation just on node
         else 
            return left_right_rotate(node); //rotate childe left and current node right
    }
    if (balance_factor < -1) { //right too long
        if (get_balance_factor(node->right) <= 0) //if right child of the node is also too lonh
            return left_rotate(node); //perform fully left rotation
         else 
            return right_left_rotate(node); //rotate childe right and current node left
    }
    node->level = 1 + Max(level(node->left), level(node->right)); //update level
    return node; //balanced node
}
BookNode *right_rotate(BookNode *root) { //rotate right
    BookNode* new_root = root->left; //left subtree root is new root
    BookNode* new_rootR = new_root->right;
    new_root->right = root; //original root is now right child of new root
    root->left = new_rootR; //righ child of the new root is now left child of original root
    
    root->level = 1 + Max(level(root->left), level(root->right));
    new_root->level = 1 + Max(level(new_root->left), level(new_root->right));
    return new_root;
}
BookNode* left_rotate(BookNode* root) { //rotate left
    BookNode* new_root = root->right; //right subtree root is new root
    BookNode* new_rootL = new_root->left;
    new_root->left = root; //original root is now left child of new root
    root->right = new_rootL; //left child of new root is now right child of original root
    
    root->level = 1 + Max(level(root->left), level(root->right));
    new_root->level = 1 + Max(level(new_root->left), level(new_root->right));
    
    return new_root;
}
BookNode* right_left_rotate(BookNode* root) { //rotate child right then node left
    root->right = right_rotate(root->right);
    return left_rotate(root);
}
BookNode* left_right_rotate(BookNode* root) { //rotate child left then node right
    root->left = left_rotate(root->left);
    return right_rotate(root);
}
//STATISTIC FUNCTIONS:
//1:
int check_balance(BookNode *node) { //function to check if tree is balanced
    if (!node) //if NULL, indicating NULL subtree is balanced
      return 1;
    int left_height = calculate_height(node->left);
    int right_height = calculate_height(node->right);
    if (abs(left_height - right_height) <= 1 && check_balance(node->left) && check_balance(node->right)) //continue checking not already unbalanced (path not too long)
        return 1;
    else
        return 0;
}
int calculate_height(BookNode *node) { //longest path from root to a leaf node (number of its levels)
    if (!node) //if NULL, level is 0
     return 0;
    return 1 + Max(calculate_height(node->left), calculate_height(node->right)); //returns longest path plus the current 1
}
//2:
int count_nodes(BookNode *node) { //count of nodes in the whole tree
    if (!node) //if NULL, there's 0 nodes
     return 0;
    return 1 + count_nodes(node->left) + count_nodes(node->right); //adds 1 recursively for each node in the BST
}
//3:
float calculate_avg_traversal(BookNode *node) { //calculate average node tranversal (how long paths are, amount of levels)
    int total_levels = 0; //initialize levels
    int total_nodes = 0; //intialize nodes
    calculate_sum(node, 0, &total_levels, &total_nodes); //calulate number of levels in the tree
    if (total_nodes > 0)
        return (float)total_levels / total_nodes; //return the average by dividing by total nodes
    else
        return 0.0;
}
void calculate_sum(BookNode *node, int level, int *total_levels, int *total_nodes) {
    if (!node) //if NULL, ends
     return;
    *total_levels += level; //adds level as the pass thorugh nodes in the tree
    (*total_nodes)++; //increases node count 
    calculate_sum(node->left, level + 1, total_levels, total_nodes); //goes thought all left nodes
    calculate_sum(node->right, level + 1, total_levels, total_nodes); //goes thought all right nodes
}
//4:
int check_unique_ids(BookNode *node, int *visited_ids, int index) { //checks IDs in the document
    if (!node) //if NULL
     return 1;
    for (int i = 0; i < index; i++) {//check ID is already in the array
        if (visited_ids[i] == node->id) 
         return 0; //there's duplicate
    }
    visited_ids[index] = node->id; //store ID of the already visited node in the array  
    if (check_unique_ids(node->left, visited_ids, index + 1) && check_unique_ids(node->right, visited_ids, index + 1)) //check next tree nodes
        return 1;
    else
        return 0;
}
