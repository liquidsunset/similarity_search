#ifndef JACCARD_H
#define JACCARD_H

#include <vector>

// are sets similar with respect to a given threshold?
inline bool jaccard(const std::vector<int> &r1, const std::vector<int> &r2, unsigned int overlapthres,
                    unsigned int posr1 = 0, unsigned int posr2 = 0, unsigned int foundoverlap = 0) {
    // taken from original implementation (verify.h), including optimizations

    unsigned int maxr1 = r1.size() - posr1 + foundoverlap;
    unsigned int maxr2 = r2.size() - posr2 + foundoverlap;

    while (maxr1 >= overlapthres && maxr2 >= overlapthres && foundoverlap < overlapthres) {
        if (r1[posr1] == r2[posr2]) {
            ++posr1;
            ++posr2;
            ++foundoverlap;
        } else if (r1[posr1] < r2[posr2]) {
            ++posr1;
            --maxr1;
        } else {
            ++posr2;
            --maxr2;
        }
    }

    bool setsAreSimilar = foundoverlap >= overlapthres;

//    std::cout << "threshold: " << threshold << " overlapthres: " << overlapthres
//              << " foundoverlap: " << foundoverlap
//              << "\tsets are similar: " << setsAreSimilar << std::endl;
    return setsAreSimilar;
}

#endif //JACCARD_H
