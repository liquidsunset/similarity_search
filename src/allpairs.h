#ifndef ALLPAIRS_H
#define ALLPAIRS_H

#include <sparsehash/dense_hash_map>
#include "jaccard.h"


typedef google::dense_hash_map<int, std::vector<int>> inverted_list;
typedef inverted_list::iterator list_iterator;

inline static unsigned int maxprefix(unsigned int len, double threshold);
inline static unsigned int minoverlap(unsigned int len1, unsigned int len2, double threshold);

struct record {
    unsigned int candidate_count = 0;
    std::vector<int> tokens;
};

void allPairs(std::vector<int> &set_vector, int set_idx, double jaccard_threshold, inverted_list &inv_list,
              std::vector<record> &all_sets) {

    std::vector<int> candidate_indexes;

    unsigned int maxpref = maxprefix(set_vector.size(), jaccard_threshold);

    //for (auto i = set_vector.begin(); i != set_vector.end(); ++i) {
    for (unsigned recpos = 0; recpos < maxpref; ++recpos) {

        int i = set_vector[recpos];
        list_iterator token_id = inv_list.find(i);

        if (token_id != inv_list.end()) {
//        std::cout << "found in I: " << token_id.first << std::endl;
            // add set indexes, where token occurs in, to candidate set
            for (auto set = token_id->second.begin(); set != token_id->second.end(); ++set) {
                record &curr_set = all_sets.at(*set);
                unsigned int indreclen = curr_set.tokens.size();
                if(indreclen < maxpref) {
                    break;
                }
                if (curr_set.candidate_count == 0) // first check if 0, increment afterwards
                    candidate_indexes.push_back(*set);
                curr_set.candidate_count++;
            }
        }
    }

    //populate inverted list index I
    for (auto i = set_vector.begin(); i != set_vector.end(); ++i) {
        int token = *i;

        std::vector<int> inverted_list_vector;
        std::pair<list_iterator, bool> entry = inv_list.insert(std::make_pair(token, inverted_list_vector));

        entry.first->second.push_back(set_idx);
    }

    // VERIFY candidates = run jaccard
    std::cout << "set " << set_idx << " similar to sets: ";

    for (auto const &candidate_index : candidate_indexes) {
        // TODO: use number of common tokens = candidate.second
        record &candidate = all_sets.at(candidate_index);

        unsigned int lastposprobe = maxpref;

        unsigned int lastposind = maxpref;
        unsigned int recpreftoklast = set_vector[lastposprobe - 1];
        unsigned int indrecpreftoklast = candidate.tokens[lastposind - 1];

        unsigned int recpos, indrecpos;

        if (recpreftoklast > indrecpreftoklast) {
            recpos = candidate.candidate_count;
            //first position after minprefix / lastposind
            indrecpos = lastposind;
        } else {
            // First position after maxprefix / lastposprobe
            recpos = lastposprobe;
            indrecpos = candidate.candidate_count;
        }

        unsigned int min = minoverlap(set_vector.size(), candidate.tokens.size(), jaccard_threshold);

        bool similar = jaccard(set_vector, candidate.tokens, min, recpos,
                               indrecpos, candidate.candidate_count);

        if (similar) {
            std::cout << candidate_index << " ";
        }

        candidate.candidate_count = 0;
    }
    std::cout << std::endl;
}

inline static unsigned int minsize(unsigned int len, double threshold) {
    return (unsigned int) (ceil(threshold * len));
}

inline static unsigned int maxprefix(unsigned int len, double threshold) {
    return std::min(len, len - minsize(len, threshold) + 1);
}

inline static unsigned int minoverlap(unsigned int len1, unsigned int len2, double threshold) {
    return std::min(len2, std::min(len1, (unsigned int) (ceil((len1 + len2) * threshold / (1 + threshold)))));
}

#endif //ALLPAIRS_H
