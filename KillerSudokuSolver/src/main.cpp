#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <string>

#include "grid.hpp"
#include "command_line_ui.hpp"

std::vector<Grid> grids;

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
			while (std::getline(file, line)) {
				if (line.starts_with('#')) {
					continue;
				}
				size_t f = line.find(':');
				Cage cage;
				cage.sum = std::atoi(line.substr(0, f).c_str());
				for (size_t n = line.find(']', f); n < line.length();) {
					int cell = atoi(line.substr(f + 2, n - f - 2).c_str());
					f = n;
					n = line.find(']', f + 1);
					if (cells_used.contains(cell)) {
						std::cout << "Error: Cell " << cell << " already used!\n";
						return 0;
					}
					if (cell < 1 || cell > 81) {
						std::cout << "Error: Cell id " << cell << " out of bounds!\n";
						return 0;
					}
					cells_used.emplace(cell);
					cage.cells.emplace_back(cell - 1);
				}
				if (cage.cells.size() == 1) {
					grid.set_cell(((cage.cells[0] - 1) % 9) + 1, ((cage.cells[0] - 1) / 9) + 1, cage.sum);
				}
				else {
					grid.add_cage(cage);
				}
			}
			if (cells_used.size() != 81) {
				std::cout << "Error: Not all cells have been used!\n";
				return 0;
			}
			std::cout << "    " << argv[1] << " loaded!\n";
			grids.emplace_back(grid);
		}

	}
	return 0;
}
