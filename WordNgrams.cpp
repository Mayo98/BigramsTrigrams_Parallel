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
#include <omp.h>
#include <sstream>
#include <map>
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
    for (int i = 0; i < 10; i++) {
        auto [count, ngram] = q.top();
        q.pop();
        cout << ngram << ": " << count << endl;
    }
}

void WordNgrams::runWordNgrams(const string& filename) {

    cout << "Calcolo n-grammi di parole..." << endl;
    int n = this->NgramLength;
    unordered_map<string, int> histogram;

    std::ifstream inputFile(filename);
    if (!inputFile.is_open() || !inputFile.good()) {

        std::cout << "Impossibile aprire il file o l'input non e' valido." << std::endl;
        exit(EXIT_FAILURE);

    }
    //vector<string> previous_words(n - 1); //verrà usato per tenere traccia delle parole precedenti nell'elaborazione degli ngrammi

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();
    std::istringstream iss(text);

    std::vector<std::string> words(std::istream_iterator<std::string>{iss},
                                   std::istream_iterator<std::string>());

    for (size_t i = 0; i <= words.size() - n-1; i++) {
        std::string currentNgram;
        for (int j = 0; j < n; j++) {
             currentNgram += words[i + j] + " ";
        }
        histogram[currentNgram]++;
    }

    printHistogram(histogram);
}
    /*
    while (iss >> word) {
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
    std::cout<<"finito lettura"<< std::endl;
    printHistogram(histogram);
}
     */

void WordNgrams::runWordNgrams_parallel(const string& filename) {
        std::cout << "Calcolo n-grammi di parole in parallelo..." << endl;
        int n = this->NgramLength;
        unordered_map<string, int> histogram;

        std::ifstream inputFile(filename);
        if (!inputFile.is_open() || !inputFile.good()) {

            std::cout << "Impossibile aprire il file o l'input non e' valido." << std::endl;
            exit(EXIT_FAILURE);

        }

        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        std::string text = buffer.str();
        const int numThreads = omp_get_max_threads();
        size_t tot = 0;

        omp_lock_t mtx;
        omp_init_lock(&mtx);
#pragma omp parallel defaut(none) shared(tot, histogram)
        {
            int threadId = omp_get_thread_num();
            const size_t textLength = text.length();
            const size_t blockSize = (textLength + numThreads - 1) / numThreads;

            std::unordered_map<std::string, int> localHistogram;

            size_t start = blockSize * threadId;
            size_t end = std::min(start + blockSize, textLength);
            std::istringstream iss(text.substr(start, end - start));
            std::vector<std::string> words(std::istream_iterator<std::string>{iss},
                                           std::istream_iterator<std::string>());
#pragma omp critical

           tot += words.size();
#pragma omp for
            for (size_t i = 0; i < words.size() - 2; i++) {

                std::string currentNgram;
                for (int j = 0; j < n; j++) {
                    currentNgram += words[i + j] + " ";
                }
                localHistogram[currentNgram]++;

            }

#pragma omp critical(add_to_histogram)
            {
                //std::cout<<"thread id: "<<omp_get_thread_num()<<"hist size: "<<localHistogram.size()<<std::endl;
                for (auto [ngram, count]: localHistogram) {
#pragma omp atomic
                        histogram[ngram] += count;
                }


            }
#pragma omp barrier
            std::cout<<"hist parallel size: "<<histogram.size()<<std::endl;
        }
#pragma omp critical{
        printHistogram(histogram);
    }

void WordNgrams::runWordNgrams_parallel2(const string& filename) {
    std::cout << "Calcolo n-grammi di parole in parallelo..." << endl;
    int n = this->NgramLength;
    unordered_map<string, int> histogram;

    std::ifstream inputFile(filename);
    if (!inputFile.is_open() || !inputFile.good()) {

        std::cout << "Impossibile aprire il file o l'input non e' valido." << std::endl;
        exit(EXIT_FAILURE);

    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();
    const int numThreads = omp_get_max_threads();
    std::istringstream iss(text);

    std::vector<std::string> words(std::istream_iterator<std::string>{iss},
                                   std::istream_iterator<std::string>());

    int start_idx = 0;
    int end_idx = words.size();
#pragma omp parallel defaut(none) shared(histogram, start_idx, end_idx)
    {
        int numThreads = omp_get_num_threads();
        int threadNum = omp_get_thread_num();

        const size_t textLength = text.length();
        int itemsPerThread = (end_idx - start_idx + 1) / numThreads;
        std::unordered_map<std::string, int> localHistogram;

        int threadStartIdx = start_idx + threadNum * itemsPerThread;
        int threadEndIdx = threadStartIdx + itemsPerThread - 1;

        if (threadNum == numThreads - 1) {
            // The last thread may have extra items.
            threadEndIdx = end_idx -1;
        }
        for (int i = threadStartIdx; i <= threadEndIdx-n-1; i++) {

            std::string currentNgram;

            for (int j = i; j < i + n; j++) {
                currentNgram += words[j] + " ";
            }
            localHistogram[currentNgram]++;

        }

#pragma omp critical(updateHistogram)
        {
            for (auto [ngram, count]: localHistogram) {
                histogram[ngram] += count;
            }

        }
#pragma omp barrier
    }

    printHistogram(histogram);
}







