#ifndef A05_UNSORTEDWORDLIST_H
#define A05_UNSORTEDWORDLIST_H
#include "Node.h"
class UnsortedWordList {
public:
    UnsortedWordList(); // default constructor
    ~UnsortedWordList(); // destructor, frees memory space
    bool IsEmpty() const; // returns true if the list is empty, else false
    int Length() const; // returns the number of nodes in the list
    Node *Front() const; // returns the first node in the list
    Node *Back() const; // returns the last node in the list
    bool Contains(string word); // returns true if the list contains the word <word>, else wrong
    Node * FindWord(string word);
// returns the pointer to the node with the word <word>
// if there is no node with that word, it returns nullptr
    string MaxWord() const;
// if exactly one word with the highest count exists -- returns that word with the highest count
// if multiple words exist that all have the maximum count -- returns "undefined" (without the quotes)
// if the list is empty -- returns "undefined" (without the quotes)
    int MaxCount() const; // returns the highest count
    void Print() const;
// Will print a summary of the list.
// Words are printed in order, as in the list, which might not be alphabetical.
// The format is, e.g., as follows (for a list with words aaa ccc bbb, with counts 2 5 1):
// WordList(
// length=3
// Word(aaa,2)
// Word(ccc,5)
// Word(bbb,1)
// )
    void CountWord(string word);
// counts the word <word>, numReadFiles.e., searches the list to find the first node that has word <word>
// If such a node is found -- the counter of that node incremented by 1
// Otherwise -- adds a new node with word <word> at the end of the list
    void InsertFirst(string word);
// If such a node is not found, creates a new node with word <word> and inserts it at the beginning of the list.
// sets count to 1.
    void DeleteAll();
// deletes all nodes in the list, the UnsortedWordList object is NOT deleted/destroyed
    void DeleteWord(string word);
// ADVANCED
// deletes node with word <word> and updates other pointers etc
protected:
    int length; // number of nodes in the list
    Node *first; // pointer to first node in the list
    Node *last; // pointer to last node in the list
// if there is only one Node in the list --> first and last point to the same Node
// if the list is empty --> first and last are set to nullptr
};
#endif //A05_UNSORTEDWORDLIST_H
