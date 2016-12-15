#ifndef ALLPAIRS_H
#define ALLPAIRS_H

typedef std::map<int, std::vector<int>>::iterator I_it;

void allPairs(std::vector<int> set_vector, int set_idx, double jaccard_threshold, std::map<int, std::vector<int>> &I,
              std::vector<std::vector<int>> &all_set_vectors) {
  std::map<int, int> candidates;
//  TODO: change to
//  std::vector<int> candidate_indexes;
//  std::vector<Set> sets;
//      Set->candidate-count (markierung, ob bereits gesehen)
//      Set->set-vector (tokens)
//  also ein eigenes struct nicht für die candidates, aber für die Sets

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

  // VERIFY candidates = run jaccard
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

#endif //ALLPAIRS_H
