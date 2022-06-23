#pragma once
#include <vector>
#include <set>

class Cage {
public:
	short sum;
	std::vector<short> cells; //[0:80]
};

class Grid {
public:
	Grid();

public:
	short get_cell(int x, int y) const; //x[1:9] y[1:9]
	void set_cell(int x, int y, short value); //x[1:9] y[1:9] value[1:9]
	void add_cage(const Cage& cage);
	std::vector<Cage> get_cages() const;
	Cage get_cage(int x, int y) const; //x[1:9] y[1:9]
	std::set<short> get_candidates(int x, int y) const; //x[1:9] y[1:9]

private:
	short grid[81]; //row major
	std::set<short> candidates[81]; //row major
	std::vector<Cage> cages; //list of cells that have to add up to a given sum
};