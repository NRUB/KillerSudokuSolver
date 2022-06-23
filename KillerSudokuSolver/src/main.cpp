#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <stack>
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
					grid.set_cell(((cage.cells[0]) % 9) + 1, ((cage.cells[0]) / 9) + 1, cage.sum);
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

		while (true) { //main loop
			system("cls");
			bool solved = true;
			bool unsolvable = false;
			for (int y = 1; y <= 9 && !unsolvable; ++y) {
				for (int x = 1; x <= 9 && !unsolvable; ++x) {
					if (grid.get_cell(x, y) == 0) {
						solved = false;
						if (grid.get_candidates(x, y).size() == 0) {
							unsolvable = true;
						}
					}
					std::set<short> candidates = grid.get_candidates(x, y);
					if (candidates.size() == 1) {
						grid.set_cell(x, y, *candidates.begin());
						x = 0;
						y = 0;
						solved = true;
						break;
					}
				}
			}
			draw_grid(grid);

			if (solved || unsolvable) {
				if (solved) {
					std::cout << "Sudoku solved!\n";
				}
				if (unsolvable) {
					std::cout << "This grid is unsolvable!\n";
				}
				if (grids.size() > 1) {
					std::cout << "There are more unfinished grids, do you wish to continue? y/n\n";
				}
				else {
					return 0;
				}
			}
			else {
				std::cout << "         q - quit\n";
				std::cout << "branch x:y - branch from a cell (x[1;9], y[1;9])\n";
				std::cout << "fill x:y v - fill a cell with a value (x[1;9], y[1;9] v[1,9])\n";
			}

			std::string command;
			std::getline(std::cin, command);
			if (command.compare("n") == 0 && (solved || unsolvable)) {
				return 0;
			}
			if (command.compare("y") == 0 && (solved || unsolvable)) {
				grid = grids.back();
				grids.pop_back();
				continue;
			}
			if (command.compare("q") == 0) {
				return 0;
			}
			if (command.starts_with("fill")) {
				std::string cell = command.substr(5, command.length() - 5);
				int x = atoi(cell.substr(0, 1).c_str());
				int y = atoi(cell.substr(2, 1).c_str());
				short v = atoi(cell.substr(4, 1).c_str());
				if (x >= 1 && x <= 9 && y >= 1 && y <= 9 && v >= 1 && v <= 9 && grid.get_candidates(x, y).contains(v)) {
					grid.set_cell(x, y, v);
				}
				continue;
			}
			if (command.starts_with("branch")) {
				std::string cell = command.substr(7, command.length() - 7);
				int x = atoi(cell.substr(0, 1).c_str());
				int y = atoi(cell.substr(2, 1).c_str());

				if (grid.get_cell(x, y) != 0) {
					std::cout << "Cell already has value\n";
					system("pause");
					continue;
				}

				Grid current = grid;
				Cage cage = current.get_cage(x, y);
				std::stack<std::pair<Grid, short>> stack; //poor man's recursion
				std::set<short> used_values; //no duplicate values in a cage
				int remainder = cage.sum;
				int n = 1;
				while (true) {
					for (; n <= 9; ++n) {
						int x = (cage.cells[stack.size()]) % 9 + 1;
						int y = (cage.cells[stack.size()]) / 9 + 1;
						if (current.get_candidates(x, y).contains(n) && !used_values.contains(n)) {
							if (stack.size() == cage.cells.size() - 1) { //last cell in range
								if (remainder == n) {
									Grid branch(current);
									branch.set_cell(x, y, n);
									grids.push_back(branch);
								}
								else { //doesn't add up to a sum
									continue;
								}
							}
							else {
								used_values.insert(n);
								Grid branch(current);
								stack.push({branch, n});

								current.set_cell(x, y, n);
								remainder -= n;
								if (remainder > 0) {
									n = 0;
								}
								else {
									used_values.erase(stack.top().second);
									current = stack.top().first;
									remainder += stack.top().second;
									n = stack.top().second + 1;
									stack.pop();
								}
							}
						}
					}
					if (stack.empty()) {
						break;
					}
					used_values.erase(stack.top().second);
					current = stack.top().first;
					remainder += stack.top().second;
					n = stack.top().second + 1;
					stack.pop();
				}
				grid = grids.back();
				grids.pop_back();
			}
		}
	}
	return 0;
}
