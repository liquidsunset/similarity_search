#include <iostream>
#include <algorithm>
#include "functions.h"
#include "tokenizer.h"


typedef std::vector<int16_t> int_vec_t;
typedef std::vector<unsigned int> CandidateIndices;

int main(int argc, char *argv[]) {

    test_jaccard();

    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " <filename> <#lines(sets) to find common integers(tokens,words)>\n";
        return -1;
    }

    std::ifstream infile("/home/liquid/similarity_search/dblp_first500.txt");
    int number_lines = atoi(argv[2]);

    token_frequency_map frequency_map = get_token_frequency(infile, number_lines);

    //set filepointer back to start
    infile.clear();
    infile.seekg(0, std::ios::beg);

    return 0;
}

void allPairs(int_vec_t token_set, double threshold) {


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






