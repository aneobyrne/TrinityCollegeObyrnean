#include "UnsortedWordList.h"

int main() {
    UnsortedWordList list;
    list.CountWord("Ultimate");
    list.CountWord("Question");
    list.CountWord("to");
    list.CountWord("Life");
    list.CountWord("Universe");
    list.CountWord("and");
    list.CountWord("Everything");
    list.CountWord("Question");
    list.Print();
}
