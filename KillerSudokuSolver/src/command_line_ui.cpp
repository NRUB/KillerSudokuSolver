#include <iostream>
#include <string>

#include "command_line_ui.hpp"

constexpr short cX = 8;
constexpr short cY = 4;

constexpr short _X = 9 * cX + 1;
constexpr short _Y = 9 * cY + 1;


void draw_grid(const Grid& grid) {
	char buffer[_X][_Y];
	memset(&buffer, ' ', _X * _Y);

	buffer[0][0] = '\xC9';
	buffer[_X - 1][0] = '\xBB';
	buffer[0][_Y - 1] = '\xC8';
	buffer[_X - 1][_Y - 1] = '\xBC';
	for (int i = 1; i < _X - 1; ++i) {
		buffer[i][0] = '\xCD';
		buffer[i][_Y - 1] = '\xCD';

	}
	for (int i = 1; i < _Y - 1; ++i) {
		buffer[0][i] = '\xBA';
		buffer[_X - 1][i] = '\xBA';
	}

	short cells[9][9];
	{
		int id = 0;
		for (auto a : grid.get_cages()) {
			short top_left = 81;
			for (auto c : a.cells) {
				top_left = std::min(top_left, c);
				short num = c;
				int x = num % 9;
				int y = num / 9;
				cells[x][y] = id;
			}
			std::string v = std::to_string(a.sum);
			short offset = 1;
			for (char c : v) {
				buffer[(top_left % 9) * cX + offset][(top_left / 9) * cY + 1] = c;
				++offset;
			}
			++id;
		}
	}

	{
		/////////
		// A B //
		// C D //
		/////////
		constexpr short AB = 0x1; // 0001
		constexpr short CD = 0x2; // 0010
		constexpr short AC = 0x4; // 0100
		constexpr short BD = 0x8; // 1000
		short cross[8][8] = {};
		for (int y = 0; y < 8; ++y) {
			for (int x = 0; x < 8; ++x) {
				if (cells[x][y] == cells[x + 1][y]) {
					cross[x][y] |= AB;
				}
				if (cells[x][y] == cells[x][y + 1]) {
					cross[x][y] |= AC;
				}
				if (cells[x + 1][y] == cells[x + 1][y + 1]) {
					cross[x][y] |= BD;
				}
				if (cells[x][y + 1] == cells[x + 1][y + 1]) {
					cross[x][y] |= CD;
				}

				char symbol = '*';
				switch (cross[x][y]) {
					case 0x0:
						symbol = '\xCE';
						break;
					case AB | CD:
						symbol = '\xCD';
						break;
					case AC | BD:
						symbol = '\xBA';
						break;
					case AB | AC:
						symbol = '\xC9';
						break;
					case AB | BD:
						symbol = '\xBB';
						break;
					case BD | CD:
						symbol = '\xBC';
						break;
					case AC | CD:
						symbol = '\xC8';
						break;
					case AB:
						symbol = '\xCB';
						break;
					case CD:
						symbol = '\xCA';
						break;
					case AC:
						symbol = '\xCC';
						break;
					case BD:
						symbol = '\xB9';
						break;
					case AB | CD | AC | BD:
						symbol = ' ';
						break;
					default:
						throw std::exception("impossible combination");
				}
				buffer[(x + 1) * cX][(y + 1) * cY] = symbol;
			}
		}
		for (int y = 0; y < 8; ++y) {
			for (int x = 0; x < 8; ++x) {
				char up = (cross[x][y] & AB) == AB ? ' ' : '\xBA';
				char down = (cross[x][y] & CD) == CD ? ' ' : '\xBA';
				char left = (cross[x][y] & AC) == AC ? ' ' : '\xCD';
				char right = (cross[x][y] & BD) == BD ? ' ' : '\xCD';

				for (int h = 0; h < cY - 1; ++h) {
					for (int w = 0; w < cX - 1; ++w) {
						buffer[(x + 1) * cX][y * cY + h + 1] = up;
						buffer[(x + 1) * cX][(y + 1) * cY + h + 1] = down;
						buffer[x * cX + w + 1][(y + 1) * cY] = left;
						buffer[(x + 1) * cX + w + 1][(y + 1) * cY] = right;
					}
				}
			}
		}
	}

	for (int y = 1; y <= 9; ++y) {
		for (int x = 1; x <= 9; ++x) {
			int val = grid.get_cell(x, y);
			if (val != 0) {
				buffer[x * cX - 4][y * cY - 2] = val + 48; //"cast" single digit to its ASCII code
			}
		}
	}

	std::cout << "\n";
	for (int y = 0; y < _Y; ++y) { //print grid
		std::cout << "    ";
		for (int x = 0; x < _X; ++x) {
			std::cout << buffer[x][y];
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}
