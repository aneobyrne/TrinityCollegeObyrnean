#ifndef A05_NODE_H
#define A05_NODE_H
#include <string>

using namespace std;

class Node {
public:
    Node(string newWord); // constructor with word
    Node(string newWord, int newCount); // constructor with word and count
    string word;
    int count = 0; // counts of occurrences for <word>
    Node * link = nullptr; // pointer to the next Node in the list
};
#endif //A05_NODE_H
