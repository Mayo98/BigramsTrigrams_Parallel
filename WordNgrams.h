//
// Created by Giacomo Magistrato on 16/01/24.
//

#include "string"
#ifndef BIGRAMSTRIGRAMS_PARALLEL_WORDNGRAMS_H
#define BIGRAMSTRIGRAMS_PARALLEL_WORDNGRAMS_H

using namespace std;

class WordNgrams {
public:
    WordNgrams(int n) {
        NgramLength = n;
    };

    void run_word_ngrams(const string &filename);

    void run_word_ngrams_parallel(const string &filename);

    int getNgramLength() const;

private:
    int NgramLength;


};


#endif //BIGRAMSTRIGRAMS_PARALLEL_WORDNGRAMS_H
