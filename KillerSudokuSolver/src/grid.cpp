#include <string>

#include "grid.hpp"

Grid::Grid() {
	for (int i = 0; i < 81; ++i) {
		for (int n = 1; n < 10; ++n) {
			candidates[i].emplace(n);
		}
		grid[i] = 0;
	}
}

short Grid::get_cell(int x, int y) const {
	if (x >= 1 && x <= 9 && y >= 1 && y <= 9) {
		return grid[y * 9 + x - 10];
	}
	else {
		throw std::exception((std::string("out of bounds x=") + std::to_string(x) + " y=" + std::to_string(y)).c_str());
	}
}

void Grid::set_cell(int x, int y, short value) {
	if (x >= 1 && x <= 9 && y >= 1 && y <= 9 && value >= 1 && value <= 9) {
		--x;
		--y;
		grid[y * 9 + x] = value;

		for (int n = 0; n < 9; ++n) {
			candidates[y * 9 + n].erase(value); //row			
			candidates[n * 9 + x].erase(value); //column			
			candidates[((x / 3) * 3) + (n % 3) + (9 * (n / 3)) + (27 * (y / 3))].erase(value); //nonet
		}
	}
	else {
		throw std::exception((std::string("out of bounds x=") + std::to_string(x) + " y=" + std::to_string(y) + " value=" + std::to_string(value)).c_str());
	}
}

void Grid::add_cage(const Cage& cage) {
	cages.emplace_back(cage);

	if (cage.cells.size() == 2) { //can possibly reduce number of candidates
		std::set<short> invalid{1,2,3,4,5,6,7,8,9};
		for (int a = 9; a >= 1; --a) {
			int b = cage.sum - a;
			if (b >= 1 && b <= 9 && a != b) {
				invalid.erase(a);
				invalid.erase(b);
			}
		}
		for (int i : cage.cells) {
			for (short s : invalid) {
				candidates[i].erase(s);
			}
		}
	}
}

std::vector<Cage> Grid::get_cages() const {
	return cages;
}

Cage Grid::get_cage(int x, int y) const {
	if (x >= 1 && x <= 9 && y >= 1 && y <= 9) {
		int index = (y * 9 + x - 10);
		for (Cage r : cages) {
			if (std::find(r.cells.begin(), r.cells.end(), index) != r.cells.end()) {
				return r;
			}
		}
	}
	else {
		throw std::exception((std::string("out of bounds x=") + std::to_string(x) + " y=" + std::to_string(y)).c_str());
	}
	throw std::exception((std::string("cell without a cage x=") + std::to_string(x) + " y=" + std::to_string(y)).c_str());
}

std::set<short> Grid::get_candidates(int x, int y) const {
	if (x >= 1 && x <= 9 && y >= 1 && y <= 9) {
		return candidates[y * 9 + x - 10];
	}
	else {
		throw std::exception((std::string("out of bounds x=") + std::to_string(x) + " y=" + std::to_string(y)).c_str());
	}
}
