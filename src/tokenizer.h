#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sparsehash/dense_hash_map>

struct eqstr {
    bool operator()(const char *s1, const char *s2) const {
        return (s1 == s2) || (s1 && s2 && strcmp(s1, s2) == 0);
    }

    bool operator()(const std::string &s1, const std::string &s2) const {
        return (s1.c_str() == s2.c_str()) || (s1 == s2);
    }
};

typedef google::dense_hash_map<std::string, int, std::tr1::hash<std::string>,
        eqstr> token_frequency_map;


struct token {
    std::string token_name;
    unsigned int count;

    bool operator<(const token &token_right) const {
        return count < token_right.count;
    }
};

token_frequency_map inline get_token_frequency(std::ifstream &file, int number_lines) {

    token_frequency_map frequency_map;
    frequency_map.set_empty_key(std::string(""));

    std::string line;    // = set
    int index = 0;
    while (std::getline(file, line) && index < number_lines) {
        std::istringstream line_stream(line);

        std::string word;    // = token
        while (line_stream >> word) {
            token_frequency_map::iterator it = frequency_map.find(word);
            if (it != frequency_map.end()) {
                it->second += 1;
            } else {
                frequency_map[word] = 1;
            }
        }
        ++index;
    }

    std::vector<token> token_vector;

    for (auto const &token_count : frequency_map) {
        token token;
        token.token_name = token_count.first;
        token.count = token_count.second;
        token_vector.push_back(token);
    }

    std::sort(token_vector.begin(), token_vector.end());

    int occurrence = 1;

    for (auto const &element : token_vector) {
        frequency_map[element.token_name] = occurrence;
        occurrence++;
    }

    return frequency_map;
}


#endif //TOKENIZER_H
