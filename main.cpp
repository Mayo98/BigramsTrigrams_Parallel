#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>
#include <chrono>
#include <functional>
#include <fstream>
#include "WordNgrams.h"
#include "CharacterNgrams.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>

#include <locale>

using namespace std;

int clean_txt() {

    // Funzione per rimuovere punteggiatura e portare testo in minuscolo
    string filename = "./input.txt";
    ifstream input(filename);
    if (!input.is_open()) {
        //std::cout << "Current path is " << std::filesystem::current_path() << '\n'; // (1)
        cout << "Impossibile aprire il file o input non valido." << endl;
        return 1;
    }
    string word;
    ofstream output("../clean_input.txt");
    if (!output.is_open()) {
        cout << "Errore nell'apertura file di output" << endl;
        return 1;
    }
    int count;
    while (input >> word) {


            string clean_word;
            for (char c: word) {
                if (!ispunct(c) && std::isalnum(c)) {  //true e carattere è un segno di punteggiatura
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

double averageWSequentialExecutions(int n, std::string path) {
    int num_test = 2;
    double mediaS;
    WordNgrams w(n);
    double sum;
    for (int i = 0; i < num_test; i++) {
        auto startS = std::chrono::steady_clock::now();
        w.runWordNgrams(path);
        //w.runWordNgrams_parallel2(path);
        auto endS = std::chrono::steady_clock::now();
        auto durationS = std::chrono::duration_cast<std::chrono::milliseconds>(endS - startS);
        std::cout << "Tempo di esecuzione " << n << "-gramma di parole Sequenziale: " << durationS.count()
                  << " millisecondi" << std::endl;
        sum += static_cast<double>(durationS.count());
        std::cout << "<<------------------------------>>" << std::endl;

    }
    mediaS = static_cast<double>(sum) / num_test;
    return mediaS;
}
double averageCSequentialExecutions(int n, std::string path) {
    //w.runWordNgrams("../clean_input.txt");
    int num_test = 1;
    double mediaS;
    double sum;
    CharacterNgrams c(n);
    for(int i =0; i < num_test; i++) {
        auto startS = std::chrono::steady_clock::now();

        c.runCharacterNgrams(path);

        auto endS = std::chrono::steady_clock::now();
        auto durationS = std::chrono::duration_cast<std::chrono::milliseconds>(endS - startS);
        std::cout << "Tempo di esecuzione "<<n<<"-gramma di caratteri Sequenziale: " << durationS.count() << " millisecondi" << std::endl;
        std::cout << "<<------------------------------>>" << std::endl;
        sum += static_cast<double>(durationS.count());
    }
    mediaS = static_cast<double>(sum) / num_test;
    return mediaS;

}
double averageCParallelExecutions(int n, std::string path) {
    //w.runWordNgrams("../clean_input.txt");
    int num_test = 1;
    double mediaP;
    double sum;
    CharacterNgrams c(n);
    for(int i =0; i < num_test; i++) {
        auto startP = std::chrono::steady_clock::now();

        c.runCharacterNgrams_parallel(path);

        auto endP = std::chrono::steady_clock::now();
        auto durationP = std::chrono::duration_cast<std::chrono::milliseconds>(endP - startP);
        std::cout << "Tempo di esecuzione "<<n<<"-gramma di caratteri Parallelo: " << durationP.count() << " millisecondi" << std::endl;
        std::cout << "<<------------------------------>>" << std::endl;
        sum += static_cast<double>(durationP.count());
    }
    mediaP = static_cast<double>(sum) / num_test;
    return mediaP;

}


double averageWParallelExecutions(int n, std::string path) {
    int num_test = 2;
    double mediaP;
    WordNgrams w(n);
    double sum;
    for (int i = 0; i < num_test; i++) {
        auto startP = std::chrono::steady_clock::now();
        w.runWordNgrams_parallel2(path);
        auto endP = std::chrono::steady_clock::now();
        auto durationP = std::chrono::duration_cast<std::chrono::milliseconds>(endP - startP);
        std::cout << "Tempo di esecuzione " << n << "-gramma di parole Parallelo: " << durationP.count() << " millisecondi" << std::endl;
        sum += static_cast<double>(durationP.count());
        std::cout << "<<------------------------------>>" << std::endl;

    }
    mediaP = static_cast<double>(sum) / num_test;
    return mediaP;
}
int main() {

    int n = 3; //n-gramma
    clean_txt();
    /*
    auto mediaS = averageWSequentialExecutions(n, "../clean_input.txt");

    auto mediaP = averageWParallelExecutions(n, "../clean_input.txt");
    std::cout << "Media esecuzione "<<n<<"-gramma parole Sequenziale : " << mediaS << std::endl;
    std::cout << "Media esecuzione "<<n<<"-gramma parole Parallelo: " << mediaP << std::endl;

    double speedup = static_cast<double>(mediaS) / static_cast<double>(mediaP);

    std::cout << "Speedup: " << speedup << std::endl;
     */
    auto mediaS = averageCSequentialExecutions(n, "../clean_input.txt");
    auto mediaP = averageCParallelExecutions(n, "../clean_input.txt");
    std::cout << "Media esecuzione "<<n<<"-gramma parole Sequenziale : " << mediaS <<" millisecondi " <<std::endl;
    std::cout << "Media esecuzione "<<n<<"-gramma parole Parallelo: " << mediaP <<" millisecondi " <<std::endl;
    double speedup = static_cast<double>(mediaS) / static_cast<double>(mediaP);
    std::cout << "Speedup: " << speedup << std::endl;
    return 0;
}
