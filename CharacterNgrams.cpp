//
// Created by Giacomo Magistrato on 19/02/24.
//

#include "CharacterNgrams.h"
#include <iostream>
#include <functional>
#include <string>
#include <sstream>
#include <locale>
#include <omp.h>
void printCharacterHistogram(unordered_map<string, int> histogram) {
    string ngram;
    int count;

    //ordino in modo decrescente
    auto startS = std::chrono::steady_clock::now();
    priority_queue<pair<int, string>> q;

    for (auto& [ngram, count]: histogram) {
        count++;
        q.push({count, ngram});
    }
    std::cout<<count<<std::endl;

    // stampo n-grammi più numerosi

    for (int i = 0; i < 4; i++) {
        auto [count, ngram] = q.top();
        q.pop();
        cout << ngram << ": " << count << endl;
    }
    auto endS = std::chrono::steady_clock::now();
    auto durationS = std::chrono::duration_cast<std::chrono::milliseconds>(endS - startS);
    std::cout << "Tempo di esecuzione print " << durationS.count() << " millisecondi" << std::endl;
}
/* VALIDO --
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

*/
void CharacterNgrams::runCharacterNgrams(const string& filename) {
    //std::locale loc;
    cout << "Calcolo n-grammi di caratteri ..." << endl;
    unordered_map<string, int> histogram;
    int n = getNgramLength();
    ifstream inputFile(filename);
    if (!inputFile) {
        cout << "Errore nell'apertura file di input" << endl;
        return;
    }
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();
    //std::cout<< "txt.lenght(): "<<text.length()<<std::endl;

    for (size_t i = 0; i <= text.length() - n ; i++) {
        bool valid = true;
        std::string currentNgram = text.substr(i, n);
        for (char c: currentNgram) {
            if (!std::isalnum(c)) {
                valid = false;
                break;
            }
        }
        if(valid) {
            histogram[currentNgram]++;
        }
    }
    printCharacterHistogram(histogram);
}

vector<string> CharacterNgrams::extractNgramsFromWord(const string& word) {

    int n = getNgramLength();
    vector<string> ngrams;
    //std::cout<<word.size()<<std::endl;
    if (word.size() >= n) {     //check se word è più lunga della lunghezza dell'n-gramma

        for (int i = 0; i < word.size() - (n - 1); i++) {

                ngrams.push_back(word.substr(i, n));

        }
    }

    return ngrams;
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
}
void CharacterNgrams::runCharacterNgrams_parallel(const string& filename) {
    //std::locale loc;
    cout << "Calcolo n-grammi di caratteri ..." << endl;
    unordered_map<string, int> histogram;
    int n = getNgramLength();
    ifstream inputFile(filename);
    if (!inputFile) {
        cout << "Errore nell'apertura file di input" << endl;
        return;
    }
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();
    //std::cout<< "txt.lenght(): "<<text.length()<<std::endl;

    int start_idx = 0;
    int end_idx = text.size();


#pragma omp parallel default(none) shared(start_idx, end_idx, n, text, histogram)
    {
        int numThreads = omp_get_num_threads();
        int threadNum = omp_get_thread_num();
        int itemsPerThread = (end_idx - start_idx + 1) / numThreads;
        int threadStartIdx = start_idx + threadNum * itemsPerThread;
        int threadEndIdx = threadStartIdx + itemsPerThread - 1;

        std::unordered_map<std::string, int> localHistogram;
        if (threadNum == numThreads - 1) {
            // The last thread may have extra items.
            threadEndIdx = end_idx - 1;
        }

        for (size_t i = threadStartIdx; i <= threadEndIdx - n; i++) {
            bool valid = true;
            std::string currentNgram = text.substr(i, n);
            for (char c: currentNgram) {
                if (!std::isalnum(c)) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                localHistogram[currentNgram]++;
            }
        }

#pragma omp critical(updateHistogram)
        {
            for (const auto &pair: localHistogram) {
                histogram[pair.first] += pair.second;
            }
        }

    }
    printCharacterHistogram(histogram);
}

int CharacterNgrams::getNgramLength() const {
    return NgramLength;
}