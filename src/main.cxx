#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <unordered_map>
#include "functions.h"


typedef std::vector<int16_t> int_vec_t;
typedef std::vector<unsigned int> CandidateIndices;
typedef std::unordered_map<int16_t, std::vector<int>> IndexedRecords;

// approach found on stackoverflow
// http://stackoverflow.com/a/5056797/2472398
template<typename A, typename B>
std::pair<B, A> flip_pair(const std::pair<A, B> &p) {
    return std::pair<B, A>(p.second, p.first);
}

template<typename A, typename B>
std::multimap<B, A> flip_map(const std::unordered_map<A, B> &src) {
    std::multimap<B, A> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
                   flip_pair<A, B>);
    return dst;
}


int main(int argc, char *argv[]) {

    test_jaccard();

    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " <filename> <#lines(sets) to find common integers(tokens,words)>\n";
        return -1;
    }

    IndexedRecords indexedRecords;
    std::unordered_map<std::string, int> token_frequency_map;
    std::ifstream infile(argv[1]);
    int number_lines = atoi(argv[2]);

    std::string line;    // = set
    int index = 0;

    while (std::getline(infile, line) && index < number_lines) {
        std::istringstream line_stream(line);

        std::string word;    // = token
        while (line_stream >> word) {
            // std::cout << word << std::endl;
            ++token_frequency_map[word];
        }
        ++index;
    }

    index = 0;

    //set filepointer back to start
    infile.clear();
    infile.seekg(0, std::ios::beg);

    // new map (key=frequency,value=token), ordered by key asc
    std::multimap<int, std::string> flip_token_frequency_map = flip_map(token_frequency_map);

    typedef std::map<int, std::string>::iterator it_type;

    int tempCount = -1;
    int occurrenceCount = 0;
    // set the token occurrence, high occurrence => low integer number
    for (auto iterator = flip_token_frequency_map.begin(); iterator != flip_token_frequency_map.end(); ++iterator) {
        token_frequency_map[iterator->second] = occurrenceCount;
        ++occurrenceCount;
    }

    //set occurrence count to -1 cause all the unrecognized tokens have negative numbers (-1, -2 ...)
    occurrenceCount = -1;


    while (std::getline(infile, line)) {
        std::istringstream line_stream(line);

        std::string word;    // = token
        int_vec_t tokens_per_line;
        while (line_stream >> word) {

            if (token_frequency_map.find(word) == token_frequency_map.end()) {
                token_frequency_map[word] = occurrenceCount;
                tokens_per_line.push_back(occurrenceCount);
                --occurrenceCount;
            } else {
                tokens_per_line.push_back(token_frequency_map[word]);
            }
        }

        std::sort(tokens_per_line.begin(), tokens_per_line.end());

    }

    return 0;
}

void allPairs(int_vec_t token_set, double threshold, IndexedRecords &indexedRecords) {


}

void verify() {

}


void test_jaccard() {
    // START of testing Jaccard calculation
    // two sample ordered vectors of ints
    static const int tokensInts1[] = {1, 2, 4, 5, 6};
    static const int tokensInts2[] = {1, 4, 5, 6, 7, 8, 9};
    // overlap = 4
    // union = 8
    // jaccard = overlap/union = 0.5

    std::vector<int> vec1(tokensInts1, tokensInts1 + sizeof(tokensInts1) / sizeof(tokensInts1[0]));
    std::vector<int> vec2(tokensInts2, tokensInts2 + sizeof(tokensInts2) / sizeof(tokensInts2[0]));

    jaccard(vec1, vec2, 0.4);        // test jaccard calculation
    jaccard(vec1, vec2, 0.5);        // test jaccard calculation
    jaccard(vec1, vec2, 0.6);        // test jaccard calculation

    // END of testing Jaccard calculation
}






