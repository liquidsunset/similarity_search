#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main (int argc, char *argv[]) {

	if ( argc != 2 ) {
		std::cout<<"usage: "<< argv[0] <<" <filename>\n";
	}
	else {
	  std::cout << "File contents:" << std::endl << std::endl;
		std::ifstream infile(argv[1]);

		std::string line;
		while (std::getline(infile, line))
		{
	    std::istringstream iss(line);
	    std::cout << line << std::endl;
		}
		return 0;
	}
	return -1;
}
