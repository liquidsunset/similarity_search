#include <iostream>
#include <algorithm>
#include <map>
#include "allpairs.h"
#include "tokenizer.h"

std::vector<record> all_sets;
inverted_list I; // TODO: dense hash map here

int main(int argc, char *argv[]) {

    if (argc != 4) {
        std::cout << "usage: " << argv[0]
                  << " <filename> <#lines(sets) to find common integers(tokens,words)> <jaccard-threshold(0..1)>\n";
        return -1;
    }

    std::ifstream infile(argv[1]);
    int number_lines = atoi(argv[2]);
    double jaccard_threshold = atof(argv[3]);

    token_frequency_map frequency_map = get_token_frequency(infile, number_lines);

    //set filepointer back to start
    infile.clear();
    infile.seekg(0, std::ios::beg);

    std::string line;
    int occurrence_count = 0;
    int set_idx = 0;

    while (std::getline(infile, line)) {
        std::istringstream line_stream(line);
        std::string word;    // = token

        record curr_record;
        std::vector<int> &tokens_per_line = curr_record.tokens;

        while (line_stream >> word) {

            std::pair<it, bool> entry = frequency_map.insert(std::make_pair(word, occurrence_count));

            if (entry.second) {
                tokens_per_line.push_back(occurrence_count);
                --occurrence_count;
            } else {
                tokens_per_line.push_back(entry.first->second);
            }
        }

        std::sort(tokens_per_line.begin(), tokens_per_line.end());

        allPairs(tokens_per_line, set_idx, jaccard_threshold, I, all_sets);

        // push current set to glocal set vector
        all_sets.push_back(curr_record);

        set_idx++;
    }
    return 0;
}