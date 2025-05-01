//==== NOTE ==================================================================
// You do not need to change anything in this file, but feel free to read it
// if it is of interest.
//
// You only need to update bstdb.c
//============================================================================

#ifndef BSTDB_H
#define BSTDB_H

typedef struct BookNode { //define the book node for the BST
    int id; //unique book ID
    char *name; //book title
    int word_count; //word count of book
    char *author; //author of the book
    int level; //for balancing purposes (which level it is at)
    struct BookNode *left, *right; //pointers to neighbours of the node
} BookNode;

int   bstdb_init           ( void );
int   bstdb_add            ( char *name, int word_count , char *author);
int   bstdb_get_word_count ( int doc_id );
char* bstdb_get_name       ( int doc_id );
void  bstdb_stat           ( void );
void  bstdb_quit           ( void );

//balancing functions (AVL-related)
BookNode* balance_tree(BookNode* node); 
BookNode* left_rotate(BookNode* root);
BookNode* right_rotate(BookNode* root);
BookNode* right_left_rotate(BookNode* root);
BookNode* left_right_rotate(BookNode* root);

//statistics
int check_balance(BookNode *node);
int count_nodes(BookNode *node);
float calculate_avg_traversal(BookNode *node);
int check_unique_ids(BookNode *node, int *visited_ids, int index);

#endif
