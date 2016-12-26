#ifndef ALLPAIRS_H
#define ALLPAIRS_H

#include <sparsehash/dense_hash_map>
#include "jaccard.h"


typedef google::dense_hash_map<unsigned int, std::vector<int>> inverted_list;
typedef inverted_list::iterator it;

struct record {
    unsigned int candidate_count = 0;
    std::vector<int> tokens;
};

void allPairs(std::vector<int> &set_vector, int set_idx, double jaccard_threshold, inverted_list &I,
              std::vector<record> &all_sets) {

    std::vector<int> candidate_indexes;

    for (auto i = set_vector.begin(); i != set_vector.end(); ++i) {
        //check against existing sets in inverted list
        for (auto const &token_id : I) {  // TODO: change to find call, TODO: dense hash map, TODO: maxprefix berechnen
            if (token_id.first == *i) {
//        std::cout << "found in I: " << token_id.first << std::endl;
                // add set indexes, where token occurs in, to candidate set
                for (auto set = token_id.second.begin(); set != token_id.second.end(); ++set) {
                    record &curr_set = all_sets.at(*set);
                    if (curr_set.candidate_count == 0) // first check if 0, increment afterwards
                        candidate_indexes.push_back(*set);
                    curr_set.candidate_count++;
                }
                break;   //token unique in I
            }
        }
    }

    //populate inverted list index I
    for (auto i = set_vector.begin(); i != set_vector.end(); ++i) {
        int token = *i;

        std::vector<int> inverted_list_vector;
        std::pair<it, bool> entry = I.insert(std::make_pair(token, inverted_list_vector));

        entry.first->second.push_back(set_idx);

        //TODO: REfactor with google maps
        /*
        I_it it = I.find(token);
        if (it != I.end()) {
          // token exists in I, push set_idx to list
          it->second.push_back(set_idx);
        } else {
          // token does not exist yet, add to I with current set_idx
          std::vector<int> vector;
          vector.push_back(set_idx);
          I.insert(std::pair<int, std::vector<int>>(token, vector));
        }*/

    }

    // VERIFY candidates = run jaccard
    std::cout << "set " << set_idx << " similar to sets: ";

    for (auto const &candidate_index : candidate_indexes) {
        // TODO: use number of common tokens = candidate.second
        record &candidate = all_sets.at(candidate_index);
        unsigned int lastposprobe = maxprefix - 1;

        unsigned int recpreftoklast = set_vector[lastposprobe - 1];
        unsigned int indrecpreftoklast = indexrecord.tokens[lastposind - 1];

        if (recpreftoklast > indrecpreftoklast) {  //TODO

            recpos = candidateData.count;
            //first position after minprefix / lastposind
            indrecpos = lastposind;
        } else {
            // First position after maxprefix / lastposprobe
            recpos = lastposprobe;
            indrecpos = candidateData.count;
        }

        bool similar = jaccard(set_vector, candidate, jaccard_threshold);
        if (similar)
            std::cout << candidate_index << " ";

        //reset candidate_count, correct? TODO
        candidate.candidate_count = 0;
    }
    std::cout << std::endl;
}

#endif //ALLPAIRS_H
