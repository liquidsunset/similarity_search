#include <iostream>
#include <algorithm>
#include <map>
#include <chrono>
#include "allpairs.h"
#include "tokenizer.h"

#define WRITE

std::vector<record> all_sets;
inverted_list inv_list;

bool write_to_file(const sim_sets &sim, const std::string &file);

int main(int argc, char *argv[]) {

    if (argc != 4) {
        std::cout << "usage: " << argv[0]
                  << " <filename> <#lines(sets) to find common integers(tokens,words)> <jaccard-threshold(0..1)>\n";
        return -1;
    }

    std::ifstream infile(argv[1]);
    int number_lines = atoi(argv[2]);
    double jaccard_threshold = atof(argv[3]);

    sim_sets similarity_sets;

    auto start = std::chrono::steady_clock::now();

    inv_list.set_empty_key(std::numeric_limits<int>::max());
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

        allPairs(curr_record, set_idx, jaccard_threshold, inv_list, all_sets, similarity_sets);

        // push current set to glocal set vector
        all_sets.push_back(curr_record);
        set_idx++;
    }

    auto end = std::chrono::steady_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<
            std::chrono::duration<double> >(end - start).count();

    std::cout << elapsed_seconds << std::endl;

    std::cout << similarity_sets.size() << std::endl;

#ifdef WRITE
    write_to_file(similarity_sets, "output.txt");
#endif
    return 0;
}

bool write_to_file(const sim_sets &sim, const std::string &file) {
    if (sim.empty())
        return false;

    sim_sets::const_iterator i;

    if (file != "") {
        FILE *out = fopen(file.c_str(), "w");
        if (!out) {
            return false;
        }
        for (i = sim.begin(); i != sim.end(); ++i) {
            fprintf(out, "%d %d \n", i->first, i->second);
        }

        fclose(out);
    }
    return true;
}