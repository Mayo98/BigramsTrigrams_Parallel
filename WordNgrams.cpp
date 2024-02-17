//
// Created by Giacomo Magistrato on 16/01/24.
//

#include "WordNgrams.h"
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cctype>
#include <unordered_map>

using namespace std;

int WordNgrams::getNgramLength() const {
    return NgramLength;
}
void print_histogram(unordered_map<string, int> histogram) {
    string ngram;
    int count;

    //sort the histogram
    priority_queue<pair<int, string>> q;
    for (auto& [ngram, count]: histogram) {
        q.push({count, ngram});
    }

    // prinit the most common ngrams
    for (int i = 0; i < 3; i++) {
        auto [count, ngram] = q.top();
        q.pop();
        cout << ngram << ": " << count << endl;
    }
}
void WordNgrams::run_word_ngrams(const string& filename) {

    cout << "Computing word ngrams..." << endl;
    int n = this->NgramLength;
    unordered_map<string, int> histogram;

    ifstream file(filename);
    if (!file) {
        cout << "Error opening input txt file" << endl;
        return;
    }

    vector<string> previous_words(n - 1); //verrà usato per tenere traccia delle parole precedenti nell'elaborazione degli ngrammi
    int num_words = 0;
    string word;
    while (file >> word) {
        if (num_words >= n - 1) {   // control for handling the first words of the txt file
            string ngram = "";
            for (int i = 0; i < n - 1; i++) {
                ngram += previous_words[i] + " ";
            }
            ngram += word;
            histogram[ngram]++;
        }
        for (int i = 0; i < n - 2; i++) {           // used when n >=3
            previous_words[i] = previous_words[i + 1];
        }
        previous_words[n - 2] = word;   // access the last element of previous_words vector (of length n-1)
        num_words++;
    }

    print_histogram(histogram);
}

