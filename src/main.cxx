#include <iostream>
#include <algorithm>
#include <map>
#include "allpairs.h"
#include "tokenizer.h"

std::vector<set> all_sets;  //TODO: rename struct
std::map<int, std::vector<int>> I; // e.g. -23 -> 5,6   -35 -> 8,9 i.e. token with integer representation -32 appears in set/line 5 and 6 etc...

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
        std::vector<int> tokens_per_line;
        while (line_stream >> word) {

            std::pair<it, bool> entry = frequency_map.insert(std::make_pair(word, occurrence_count));

            if (entry.second) {
                tokens_per_line.push_back(occurrence_count);
                --occurrence_count;
            } else {
                tokens_per_line.push_back(entry.first->second);
            }
            /*
            if (frequency_map.find(word) == frequency_map.end()) {
                frequency_map[word] = occurrence_count;
                tokens_per_line.push_back(occurrence_count);
                --occurrence_count;
            } else {
                tokens_per_line.push_back(frequency_map[word]);
            }
            */
        }

        std::sort(tokens_per_line.begin(), tokens_per_line.end());

      allPairs(tokens_per_line, set_idx, jaccard_threshold, I, all_sets);

      // push current set to glocal set vector
      set set;  //TODO: do not copy vector
      set.tokens = tokens_per_line;
      set.candidate_count = 0;
      all_sets.push_back(set);

      set_idx++;
    }
    return 0;
}