#pragma once
class Sudoku
{
public:
	int sudo[9][9];
	int avlb[9][9][9];
	Sudoku(int insudo[9][9]);
	bool Check(int col, int raw, int num);
	bool Callback(int col, int raw);
};

