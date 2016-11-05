#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <iterator>
#include <string>	
#include <map>
#include <vector>
#include <algorithm>

// approach found on stackoverflow
// http://stackoverflow.com/a/5056797/2472398
template<typename A, typename B>
std::pair<B,A> flip_pair(const std::pair<A,B> &p)
{
    return std::pair<B,A>(p.second, p.first);
}

template<typename A, typename B>
std::multimap<B,A> flip_map(const std::map<A,B> &src)
{
    std::multimap<B,A> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()), 
                   flip_pair<A,B>);
    return dst;
}

// returns the Jaccard index of 2 given sets (i.e. lines)
float jaccard (std::string line1, std::string line2) {	// = set1, set2
	std::istringstream line1_stream(line1);
	std::istringstream line2_stream(line2);
  std::map<std::string, int> words;
  int overlap = 0;

	std::string word;	// = token

	// insert all words of line1
  while (line1_stream >> word) {
  	++words[word];
		if (words.count(word) == 0) {
			++words[word];
  	}  	
  }
  overlap += words.size();

  // insert new words of line2 and increment overlap on existing words
  while (line2_stream >> word) {
		if (words.count(word) == 0) {
			++words[word];
  	} else {
  		++overlap;
  	}
  }

  float jaccard = (float)overlap/(float)words.size();

  std::cout << "jaccard (overlap/union): " << jaccard << std::endl;
  return jaccard;
}

int main (int argc, char *argv[]) {

	if ( argc != 3 ) {
		std::cout<<"usage: "<< argv[0] <<" <filename> <#lines(sets) to find common integers(tokens,words)>\n";
	}
	else {
		std::map<std::string, int> token_frequency_map;
		std::ifstream infile(argv[1]);
		int number_lines = atoi(argv[2]);

		std::string previous_line;	// = set, used for testing jaccard calculation
		std::string line;	// = set
		int index = 0;

		while (std::getline(infile, line) && index < number_lines)
		{
    	std::istringstream line_stream(line);

	    std::string word;	// = token
	    while (line_stream >> word) {
	    	// std::cout << word << std::endl;
	    	++token_frequency_map[word];
	    }
	    ++index;

	    if (!previous_line.empty())
	   		jaccard(previous_line, line);		// test jaccard calculation
	    previous_line = line;
		}

		// new map (key=frequency,value=token), ordered by key asc
    std::multimap<int, std::string> flip_token_frequency_map = flip_map(token_frequency_map);

    typedef std::map<int, std::string>::iterator it_type;
    it_type begin = flip_token_frequency_map.begin();
    it_type end = flip_token_frequency_map.end();

		for(it_type iterator = begin;iterator != end; ++iterator) {
			std::cout << iterator->first << " x " << iterator->second << std::endl;
		}

		// at this point, we have an asc ordered list of tokens, whereas the index is the unique 
		// integer we use for further processing.
		// e.g. 
		// 0 -> 1 x (URL)
		// 1 -> 1 x -
		// 2 -> 1 x 0002
		// ...
		// 496 -> 8 x Jörg
		// 497 -> 10 x The
		// 498 -> 13 x for
		// 499 -> 18 x of


		return 0;
	}
	return -1;
}
