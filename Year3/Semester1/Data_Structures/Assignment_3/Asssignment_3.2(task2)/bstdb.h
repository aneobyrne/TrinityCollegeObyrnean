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
void tree_delete(BookNode* root);
BookNode *create_node(int id, char *name, int word_count, char *author);

//ID functions:
int get_unique_id(void);
int check_uniqueness(int id, BookNode *node);

//balancing functions (AVL-related)
int level(BookNode *node);
int get_balance_factor(BookNode *node);
BookNode* balance_tree(BookNode* node); 
BookNode* left_rotate(BookNode* root);
BookNode* right_rotate(BookNode* root);
BookNode* right_left_rotate(BookNode* root);
BookNode* left_right_rotate(BookNode* root);
int Max(int a, int b);

//statistics
int check_balance(BookNode *node);
int calculate_height(BookNode *node);
int count_nodes(BookNode *node);
float calculate_avg_traversal(BookNode *node);
void calculate_sum(BookNode *node, int level, int *total_levels, int *total_nodes);
int check_unique_ids(BookNode *node, int *visited_ids, int index);


#endif
