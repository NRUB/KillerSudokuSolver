#include <iostream>
#include <set>

#include "grid.hpp"

int main(int argc, char** argv)
{
	std::cout << "=== Killer Sudoku Solver v0.1! ===\n";
	if (argc == 1) { //print help
		std::cout << "You have to supply a path to a file with your sudoku.\n";
		std::cout << "The file format used is as follow:\n\n";
		std::cout << "xx:[n][n][n][n][n]\n\n";
		std::cout << "Where:\n";
		std::cout << " - 'xx' is a sum for a cage\n";
		std::cout << " - 'n' is a cell id\n";
		std::cout << "Cells are numbered as follow:\n";
		std::cout << "   1  2  3  4  5  6  7  8  9\n";
		std::cout << "  10 11 12 13 14 15 ...\n";
		std::cout << "              ...\n";
		std::cout << "       ... 76 77 78 79 80 81\n";
		std::cout << "\n";
		std::cout << "Comments start with #\n";
		std::cout << "\n";
	}
	else {
		std::string line;
		std::ifstream file(argv[1], std::ios_base::binary | std::ios_base::in);

		Grid grid;
		{ //initialize grid
			std::set<short> cells_used;
		}

	}
	return 0;
}
