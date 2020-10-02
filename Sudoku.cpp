#include "pch.h"
#include "Sudoku.h"

Sudoku::Sudoku(int insudo[9][9])
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			sudo[i][j] = insudo[i][j];
		}
	}
}

bool Sudoku::Check(int col, int raw, int num)
{
	for (int i = 0; i < 9; i++) {
		if (i == raw)continue;
		if (sudo[col][i] == num)return false;
	}
	for (int i = 0; i < 9; i++) {
		if (i == col)continue;
		if (sudo[i][raw] == num)return false;
	}
	return true;
}

bool Sudoku::Callback(int col, int raw)
{
	bool know = sudo[col][raw] != 0;
	for (int i = 1; i < 10; i++) {
		if (Check(col, raw, i) && !know) {
			sudo[col][raw] = i;
		}
		else if (know) {
			i = 9;
		}
		else {
			continue;
		}
		if (col == 8 && raw == 8) {
			return true;
		}
		else if (col < 8 && raw == 8) {
			if (Callback(col + 1, 0)) {
				return true;
			}
			else {
				continue;
			}
		}
		else {
			if (Callback(col, raw + 1)) {
				return true;
			}
			else {
				continue;
			}
		}
	}
	if (know) { return false; }
	else {
		sudo[col][raw] = 0;
		return false;
	}
}
