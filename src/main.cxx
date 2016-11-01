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

int main (int argc, char *argv[]) {

	if ( argc != 2 ) {
		std::cout<<"usage: "<< argv[0] <<" <filename>\n";
	}
	else {
		std::map<std::string, int> token_frequency_map;
		std::ifstream infile(argv[1]);

		std::string line;	// = set
		while (std::getline(infile, line))
		{
	    std::cout << std::endl << std::endl;
    	std::istringstream line_stream(line);

	    std::string word;	// = token
	    while (line_stream >> word) {
	    	// std::cout << word << std::endl;
	    	++token_frequency_map[word];
	    }
		}

		// new map (key=frequency,value=token), ordered by key asc
    std::multimap<int, std::string> flip_token_frequency_map = flip_map(token_frequency_map);

    typedef std::map<int, std::string>::iterator it_type;
    it_type begin = flip_token_frequency_map.begin();
    it_type end = flip_token_frequency_map.end();

		for(it_type iterator = begin;iterator != end; ++iterator) {
			std::cout << iterator->first << " x " << iterator->second << std::endl;
		}
		return 0;
	}
	return -1;
}
