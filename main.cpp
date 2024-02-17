#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>
#include <chrono>
#include <functional>
#include <fstream>
#include "WordNgrams.h"
//#include "CharacterNgrams.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <locale>

using namespace std;

std::map<std::string, int> computeTrigramHistogram(const std::string& text) {
    std::map<std::string, int> histogram;
    std::locale loc("it_IT.UTF-8");

    for (size_t i = 1; i < text.length() - 1; ++i) {
        std::string currentTrigram = text.substr(i - 1, 3);

        if (isalpha(currentTrigram[0], loc) &&
            isalpha(currentTrigram[1], loc) &&
            isalpha(currentTrigram[2], loc)) {
            currentTrigram[0] = tolower(currentTrigram[0], loc);
            currentTrigram[1] = tolower(currentTrigram[1], loc);
            currentTrigram[2] = tolower(currentTrigram[2], loc);
            histogram[currentTrigram]++;
        }
    }

    return histogram;
}

int clean_txt() {
    // This function removes punctuation and transforms every character to lowercase
    string filename = "./input.txt";
    ifstream input(filename);

    if (!input.is_open()) {
        std::cout << "Current path is " << std::filesystem::current_path() << '\n'; // (1)
        cout << "Error opening input file" << endl;
        return 1;
    }
    string word;
    ofstream output("../clean_input.txt");
    if (!output.is_open()) {
        cout << "Error opening output file" << endl;
        return 1;
    }
    while (input >> word) {
        string clean_word;
        for (char c: word) {
            if (!ispunct(c)) {
                clean_word += tolower(c);
            }
        }
        if (!clean_word.empty()) {
            output << clean_word << " ";
        }
    }
    input.close();
    output.close();
    return 0;
}

int main() {
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open() || !inputFile.good()) {

        std::cout << "Impossibile aprire il file o l'input non e' valido." << std::endl;
        exit(EXIT_FAILURE);

    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();


    std::map<std::string, int> trigramHistogram = computeTrigramHistogram(text);
    std::cout << "\nTrigram Histogram:" << std::endl;
    for (const auto& pair : trigramHistogram) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    clean_txt();
    WordNgrams w(3);
    w.run_word_ngrams("../clean_input.txt");
    return 0;
}
