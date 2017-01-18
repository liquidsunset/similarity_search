#ifndef ALLPAIRS_H
#define ALLPAIRS_H

#include <sparsehash/dense_hash_map>
#include "jaccard.h"


typedef google::dense_hash_map<int, std::vector<int>> inverted_list;
typedef inverted_list::iterator list_iterator;
typedef std::vector<std::pair<unsigned int, unsigned int>> sim_sets;

inline static unsigned int maxprefix(unsigned int len, double threshold, unsigned int minsizeprobe);

inline static unsigned int minoverlap(unsigned int len1, unsigned int len2, double threshold);

inline static unsigned int maxsize(unsigned int len, double threshold);

inline static unsigned int minsize(unsigned int len, double threshold);

struct record {
    unsigned int candidate_count = 0;
    unsigned int maxpref;
    std::vector<int> tokens;
};

inline void allPairs(record &set_record, int set_idx, double jaccard_threshold, inverted_list &inv_list,
                     std::vector<record> &all_sets, sim_sets &similarity_sets) {

    std::vector<int> candidate_indexes;
    std::vector<int> &set_vector = set_record.tokens;

    unsigned int set_size = set_vector.size();

    int count = 0;
    unsigned int minsizeprobe = minsize(set_size, jaccard_threshold);
    unsigned int maxsizeprobe = maxsize(set_size, jaccard_threshold);
    unsigned int maxprefprobe = maxprefix(set_size, jaccard_threshold, minsizeprobe);

    set_record.maxpref = maxprefprobe;

    for (unsigned int recpos = 0; recpos < maxprefprobe; ++recpos) {

        int token = set_vector[recpos];
        list_iterator token_id = inv_list.find(token);

        if (token_id != inv_list.end()) {

            for (auto set = token_id->second.begin(); set != token_id->second.end(); ++set) {
                record &curr_set = all_sets.at(*set);

                unsigned int indreclen = curr_set.tokens.size();

                if (!(indreclen >= minsizeprobe && indreclen <= maxsizeprobe)) {
                    continue;
                }

                if (curr_set.candidate_count == 0) // first check if 0, increment afterwards
                    candidate_indexes.push_back(*set);
                curr_set.candidate_count++;
            }
        }
    }

    // VERIFY candidates = run jaccard
    //std::cout << "set " << set_idx << " similar to sets: ";

    for (auto const &candidate_index : candidate_indexes) {

        record &candidate = all_sets.at(candidate_index);
        std::vector<int> &candidate_tokens = candidate.tokens;
        unsigned int candidate_size = candidate.tokens.size();
        unsigned int lastposprobe = maxprefprobe;
        unsigned int lastposind = candidate.maxpref;

        int recpreftoklast = set_vector[lastposprobe - 1];
        int indrecpreftoklast = candidate.tokens[lastposind - 1];

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

        unsigned int min = minoverlap(set_size, candidate_size, jaccard_threshold);
        bool similar = jaccard(set_vector, candidate_tokens, min, recpos,
                               indrecpos, candidate.candidate_count);

        if (similar) {
            //std::cout << set_idx << ": " <<candidate_index << std::endl;
            similarity_sets.push_back(std::make_pair(set_idx, candidate_index));
        }

        candidate.candidate_count = 0;
    }

    //populate inverted list index I

    for (unsigned int i = 0; i < maxprefprobe; i++) {
        int token = set_vector[i];
        std::vector<int> inverted_list_vector;
        std::pair<list_iterator, bool> entry = inv_list.insert(std::make_pair(token, inverted_list_vector));

        entry.first->second.push_back(set_idx);
    }
    //std::cout << std::endl;
}

inline static unsigned int minsize(unsigned int len, double threshold) {
    return (unsigned int) (ceil(threshold * len));
}

inline static unsigned int maxprefix(unsigned int len, double threshold, unsigned int minsizeprobe) {
    return std::min(len, len - minsizeprobe + 1);
}

inline static unsigned int maxsize(unsigned int len, double threshold) {
    return (unsigned int) ((len / threshold));
}

inline static unsigned int minoverlap(unsigned int len1, unsigned int len2, double threshold) {
    return std::min(len2, std::min(len1, (unsigned int) (ceil((len1 + len2) * threshold / (1 + threshold)))));
}

#endif //ALLPAIRS_H
