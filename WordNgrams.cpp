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
void printHistogram(unordered_map<string, int> histogram) {
    string ngram;
    int count;

    //ordino l'istogramma in ordine decrescente

    priority_queue<pair<int, string>> q;
    for (auto& [ngram, count]: histogram) {
        q.push({count, ngram});
    }

    // stampo le parole più numerose
    for (int i = 0; i < 3; i++) {
        auto [count, ngram] = q.top();
        q.pop();
        cout << ngram << ": " << count << endl;
    }
}
void WordNgrams::runWordNgrams(const string& filename) {

    cout << "Calcolo n-grammi di parole..." << endl;
    int n = this->NgramLength;
    unordered_map<string, int> histogram;

    ifstream file(filename);
    if (!file) {
        cout << "Errore nell'apertura file di input" << endl;
        return;
    }

    vector<string> previous_words(n - 1); //verrà usato per tenere traccia delle parole precedenti nell'elaborazione degli ngrammi
    int num_words = 0;
    string word;
    while (file >> word) {
        if (num_words >= n - 1) {   // controllo se ho raggiunto la lunghezza giusta per formare n-gramma
            string ngram = "";
            for (int i = 0; i < n - 1; i++) {
                ngram += previous_words[i] + " ";           //riempio stringa con l'n-1 - gramma
            }
            ngram += word;                                  //aggiungo word attuale e completo n-gramma
            histogram[ngram]++;
        }
        for (int i = 0; i < n - 2; i++) {           // ciclo per spostare (destra a sinistra) le parole precedenti via via
            previous_words[i] = previous_words[i + 1];
        }
        previous_words[n - 2] = word;   // salvo word attuale in posizione n-2 (in coda)
        num_words++;                    //incremento contatore n-gramma
    }

    printHistogram(histogram);
}

