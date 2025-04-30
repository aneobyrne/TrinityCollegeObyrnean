#ifndef A05_TEXTANALYSER_H
#define A05_TEXTANALYSER_H
#include "UnsortedWordList.h"
#include <string>
using namespace std;
class TextAnalyser {
private:
    bool readIntoA = true; // determines whether ReadFile() stores the input into listA or listB
// is flipped during each ReadFile(), so ReadFile() operations alternate between A and B.
public:
    UnsortedWordList listA; // stores words read in each odd attempt (1st, 3rd, 5th, ...)
    UnsortedWordList listB; // stores words read in each even attempt (2nd, 4th, 6th, ...)
    UnsortedWordList listC; // stores result of Intersection() and Union()

    void ReadFile(string filename);
// at the beginning prints a diagnostic message of the form
// TextAnalyser::ReadFile, filename='<name of the file>'
//
// reads in a file and puts the words in listA or listB
//
// alternates readIntoA between true and false to control which list is used
    void Intersection();
// determines intersection of A and B, i.e.
// it puts all words that appear in listA *and* listB in listC
// for each word it takes the minimum of word counts in A and B for C
    void Union();
// determines union of A and B, i.e.
// it puts all words that appear in listA *or* listB in listC
// for each word it takes the sum of word counts in A and B (not the maximum!) for C
// i.e., it has the same result as reading both files that were source for A and B
};
#endif //A05_TEXTANALYSER_H
