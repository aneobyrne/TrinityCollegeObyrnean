#include "UnsortedWordList.h"
#include <iostream>

using namespace std;

UnsortedWordList::UnsortedWordList(){} // default constructor

UnsortedWordList::~UnsortedWordList(){} // destructor, frees memory space

bool UnsortedWordList::IsEmpty() const{ // returns true if the list is empty, else false
    if ((this->first == nullptr) && (this->last == nullptr))
        return true;
    else
        return false;
}

int UnsortedWordList::Length() const{ // returns the number of nodes in the list
    int length = 0;
    Node* current = this->first;
    while (current != nullptr){
        length++;
        current= current->link;
    }
    return length;
}

Node* UnsortedWordList::Front() const{ // returns the first node in the list
    return first;
}

Node *UnsortedWordList::Back() const{ // returns the last node in the list
    return last;
}

bool UnsortedWordList::Contains(string word){ // returns true if the list contains the word <word>, else wrong
    Node* current = this->first;
    while (current != nullptr){
        if (current->word == word) {
            return true;
            break;
        }
        current = current -> link;
    }
    return false;
}

Node * UnsortedWordList::FindWord(string word){ // returns the pointer to the node with the word <word>, if there is no node with that word, it returns nullptr
    Node* current = this->first;
    while (current != nullptr){
        if (current->word == word) {
            return current;
            break;
        }
        current = current -> link;
    }
    return nullptr;
}

string UnsortedWordList::MaxWord() const{
    int max = 0;
    int max2 = 0;
    Node* current = this->first;
    Node* MaxNode = this -> first;
    while (current != nullptr){
        if (current->count > max) {
            max2 = max;
            max = current->count;
            MaxNode = current;
        }
        current = current->link;
    }
    if ((max == 0)&& (max == max2))
        return "undefined";
    else
        return MaxNode->word;
}

int UnsortedWordList::MaxCount() const{ // returns the highest count
    int max = 0;
    Node* current = this->first;
    while (current != nullptr){
        if (current->count > max) {
            max = current->count;
        }
        current = current->link;
    }
    return max;
}

void UnsortedWordList::Print() const{ // Will print a summary of the list.
    cout << "WordList(" << endl;
    cout << "length=" << length << endl;
    Node* current = this->first;
    while (current != nullptr){
        cout << "Word(" << current->word << "," << current->count << ")"<< endl;
        current = current ->link;
    }
    cout << ")";
}

void UnsortedWordList::CountWord(string word){ // counts the word <word>, numReadFiles.e., searches the list to find the first node that has word <word>
    Node* current = this->first;
    bool found = false;
    while (current != nullptr){
        if (current->word == word) {
            current->count++;
            found = true;
        }
        current = current -> link;
    }

    if (!found){
        Node* NewNode = new Node(word, 1);
        if(IsEmpty()){
            this->first = NewNode;
            this->last = NewNode;
        }
        else {
            this->last->link = NewNode;
            this->last = NewNode;
        }
        this->length++;
    }
}

void UnsortedWordList::InsertFirst(string word){ // If such a node is not found, creates a new node with word <word> and inserts it at the beginning of the list.
    Node* current = this->first;
    bool found = false;
    while (current != nullptr){
        if (current->word == word) {
            current->count++;
            found = true;
        }
        current = current -> link;
    }

    if (!found){
        Node* NewNode = new Node(word, 1);
        if(IsEmpty()){
            this->first = NewNode;
            this->last = NewNode;
        }
        else {
            NewNode->link = this->first;
            this->first = NewNode;
        }
        this->length++;
    }
}

void UnsortedWordList::DeleteAll(){
    this->first = nullptr;
    this->last = nullptr;
    this->length = 0;
}

void UnsortedWordList::DeleteWord(string word){
    Node* previous = nullptr;
    Node* current = this->first;

    while (current != nullptr) {
        if (current->word == word) {
            if (previous == nullptr)
                this->first = current->link;
            else
                previous->link = current->link;
            this->length--;
            delete current;
            break;
        }
        previous = current;
        current = current->link;
    }
}
