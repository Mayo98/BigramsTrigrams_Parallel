//
// Created by Giacomo Magistrato on 16/01/24.
//

#include "string"
#ifndef BIGRAMSTRIGRAMS_PARALLEL_WORDNGRAMS_H
#define BIGRAMSTRIGRAMS_PARALLEL_WORDNGRAMS_H

using namespace std;

class WordNgrams {
private:
    int NgramLength;

public:
    WordNgrams(int n) {
        NgramLength = n;
    };

    void runWordNgrams(const string &filename);

    void runWordNgrams_parallel(const string &filename);
    void runWordNgrams_parallel2(const string &filename);
    int getNgramLength() const;




};


#endif //BIGRAMSTRIGRAMS_PARALLEL_WORDNGRAMS_H
