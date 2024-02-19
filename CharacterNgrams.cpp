//
// Created by Giacomo Magistrato on 19/02/24.
//

#include "CharacterNgrams.h"
#include <iostream>
#include <functional>
#include <string>
#include <cctype>
void printCharacterHistogram(unordered_map<string, int> histogram) {
    string ngram;
    int count;

    //ordino in modo decrescente

    priority_queue<pair<int, string>> q;
    for (auto& [ngram, count]: histogram) {
        q.push({count, ngram});
    }

    // stampo n-grammi più numerosi

    for (int i = 0; i < 10; i++) {
        auto [count, ngram] = q.top();
        q.pop();
        cout << ngram << ": " << count << endl;
    }
}

void CharacterNgrams::runCharacterNgrams(const string& filename) {

    cout << "Calcolo n-grammi di caratteri ..." << endl;
    unordered_map<string, int> histogram;

    ifstream file(filename);
    if (!file) {
        cout << "Errore nell'apertura file di input" << endl;
        return;
    }

    string word;
    vector<string> ngrams;
    while (file >> word) {      //scorro parole ed estraggo n-grammi caratteri
        ngrams = extractNgramsFromWord(word);
        for (auto ngram : ngrams) {
            histogram[ngram]++;
        }
    }
    printCharacterHistogram(histogram);
}
vector<string> CharacterNgrams::extractNgramsFromWord(const string& word) {

    int n = getNgramLength();
    vector<string> ngrams;
    //std::cout<<word.size()<<std::endl;
    if (word.size() >= n) {     //check se word è più lunga della lunghezza dell'n-gramma

     /*   for (int i = 0; i < word.size() - (n - 1); i++) {
            std::string ngram = word.substr(i, n);
            bool isAlphanumeric = true;

            // Verifica se la stringa ngram contiene solo caratteri alfanumerici
            for (char c: ngram) {
                if (!std::isalnum(c, std::locale("it_IT.UTF-8"))) {
                    isAlphanumeric = false;
                    break;
                }
            }

            // Aggiungi la stringa al vettore solo se è alfanumerica
            if (isAlphanumeric) {
                ngrams.push_back(ngram);
            }
        }
    }
    return ngrams;
}

*/

        for (int i = 0; i < word.size() - (n - 1); i++) {

                ngrams.push_back(word.substr(i, n));

        }
    }

    return ngrams;
}

int CharacterNgrams::getNgramLength() const {
    return NgramLength;
}