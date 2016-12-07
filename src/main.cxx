#include <iostream>
#include <algorithm>
#include <map>
#include "functions.h"
#include "tokenizer.h"

// will be filled during iteration,
// processed in every allPairs call
std::vector<std::vector<int>> set_vectors;
// e.g.
// -23 -> 5,6   -35 -> 8,9
// i.e. token with integer representation -32 appears in set/line 5 and 6 etc...
std::map<int, std::vector<int>> I;
typedef std::map<int, std::vector<int>>::iterator I_it;

void allPairs(std::vector<int> vector, int i);

int main(int argc, char *argv[]) {

  test_jaccard();

  if (argc != 3) {
    std::cout << "usage: " << argv[0] << " <filename> <#lines(sets) to find common integers(tokens,words)>\n";
    return -1;
  }

  std::ifstream infile(argv[1]);
  int number_lines = atoi(argv[2]);

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
      if (frequency_map.find(word) == frequency_map.end()) {
        frequency_map[word] = occurrence_count;
        tokens_per_line.push_back(occurrence_count);
        --occurrence_count;
      } else {
        tokens_per_line.push_back(frequency_map[word]);
      }
    }

    std::sort(tokens_per_line.begin(), tokens_per_line.end(), std::less<int>());


//        // debug: print line and ordered vector
//        std::cout << line << std::endl;
//        for (auto i = tokens_per_line.begin(); i != tokens_per_line.end(); ++i)
//            std::cout << *i << ' ';
//        std::cout << std::endl << std::endl;

//        // debug: print line and ordered vector
//        for (auto j = set_vectors.begin(); j != set_vectors.end();) {
//            std::vector<int> vector = *j;
//            for (auto i = vector.begin(); i != vector.end(); ++i)
//                std::cout << *i << ' ';
//            std::cout << std::endl << std::endl;
//            j++;
//        }
//        std::cout << std::endl << std::endl;


    // ALL PAIRS
    //inverted list index, jaccard
    allPairs(tokens_per_line, set_idx);

    // push current token vector to global vector
    set_vectors.push_back(tokens_per_line);

    set_idx++;
  }
  return 0;
}

void allPairs(std::vector<int> set_vector, int set_idx) {
  std::vector<int> candidates;  //TODO: also store number of common tokens

  for (auto i = set_vector.begin(); i != set_vector.end(); ++i) {
    //check against existing sets in inverted list
    for (auto const &token_id : I) {
      if (token_id.first == *i) {
        std::cout << "found in I: " << token_id.first << std::endl;
        // add set indexes, where token occurs to candidate set
        for (auto set = token_id.second.begin(); set != token_id.second.end(); ++set) {
          candidates.push_back(*set);
        }
        break;   //token only once in I
      }
    }
  }

  // print candidates (set indexes, which have at least one common token)
  std::cout << "Candidates (set indexes) for set_idx " << set_idx << ": ";
  for (auto i = candidates.begin(); i != candidates.end(); ++i) {
    std::cout << *i << ' ';
  }
  std::cout << std::endl << std::endl;

  //populate inverted list index
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

  // TODO: now verify candidates = run jaccard
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






