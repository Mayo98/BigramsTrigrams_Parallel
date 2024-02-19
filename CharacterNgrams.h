//
// Created by Giacomo Magistrato on 19/02/24.
//

#ifndef BIGRAMSTRIGRAMS_PARALLEL_CHARACTERNGRAMS_H
#define BIGRAMSTRIGRAMS_PARALLEL_CHARACTERNGRAMS_H

#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cctype>
#include <unordered_map>

using namespace std;

class CharacterNgrams {
public:
    CharacterNgrams(int n) {
        NgramLength = n;
    };

    void runCharacterNgrams(const string &filename);

    void parallel_compute_character_ngrams(const string &filename);

    vector<string> extractNgramsFromWord(const string& str);

    int getNgramLength() const;

private:
    int NgramLength;
};




#endif //BIGRAMSTRIGRAMS_PARALLEL_CHARACTERNGRAMS_H
