#include <iostream>
#include <algorithm>
#include <map>
#include "jaccard.h"
#include "tokenizer.h"

std::vector<std::vector<int>> all_set_vectors;
std::map<int, std::vector<int>> I; // e.g. -23 -> 5,6   -35 -> 8,9 i.e. token with integer representation -32 appears in set/line 5 and 6 etc...
typedef std::map<int, std::vector<int>>::iterator I_it;

void allPairs(std::vector<int> vector, int set_idx, double jaccard_threshold);

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

        allPairs(tokens_per_line, set_idx, jaccard_threshold);

        // push current token vector to global vector
        all_set_vectors.push_back(tokens_per_line);

        set_idx++;
    }
    return 0;
}

void allPairs(std::vector<int> set_vector, int set_idx, double jaccard_threshold) {
    std::map<int, int> candidates;  //TODO: also store number of common tokens

    for (auto i = set_vector.begin(); i != set_vector.end(); ++i) {
        //check against existing sets in inverted list
        for (auto const &token_id : I) {
            if (token_id.first == *i) {
//        std::cout << "found in I: " << token_id.first << std::endl;
                // add set indexes, where token occurs in, to candidate set
                for (auto set = token_id.second.begin(); set != token_id.second.end(); ++set) {
                    if (candidates.find(*set) == candidates.end()) {
                        candidates[*set] = 1;
                    } else {
                        ++candidates[*set];
                    }
                }
                break;   //token unique in I
            }
        }
    }

    //populate inverted list index I
    for (auto i = set_vector.begin(); i != set_vector.end(); ++i) {
        int token = *i;

        I_it it = I.find(token);
        if (it != I.end()) {
            // token exists in I, push set_idx to list
            it->second.push_back(set_idx);
        } else {
            // token does not exist yet, add to I with current set_idx
            std::vector<int> vector;
            vector.push_back(set_idx);
            I.insert(std::pair<int, std::vector<int>>(token, vector));
        }
    }

    // now verify candidates = run jaccard
    std::cout << "set " << set_idx << " similar to sets: ";

    for (auto const &candidate : candidates) {
        // TODO: use number of common tokens = candidate.second

        bool similar = jaccard(set_vector, all_set_vectors.at(candidate.first),
                               jaccard_threshold);
        if (similar)
            std::cout << candidate.first << " ";
    }
    std::cout << std::endl;
}



