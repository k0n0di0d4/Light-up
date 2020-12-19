#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <time.h>
int wielkosc_kwadratowa;
/*
* 0 = Puste pole
* 1 = Czarny Kwadracik z numerem 1 
* 2 = Czarny Kwadracik z numerem 2
* 3 = Czarny Kwadracik z numerem 3
* 4 = Czarny Kwadracik z numerem 4
* 5 = Lampka
* 6 = Œwiat³o
* 7 = Czarny Kwadracik bez numeru
*/
std::vector<int> v = { 1, 2, 3, 4, 7 };

void board_init(std::vector<std::vector<int>> vektor, int wk) {
	vektor.resize(wk); //table i wielkosc_kwadratowa
	for (int i = 0; i < wk; i++)
		vektor[i].resize(wk);
}

int numba_of_black_squares(int wk) {
	//ilosc czarnych kwadratow do wstawienia w tablice
	int black_numba_min = round(sqrt(wk));
	int black_numba_max = wk * black_numba_min;
	std::cout << "Minimalna iloœæ czarnych kwadratów(do losowania): " << black_numba_min << std::endl;
	std::cout << "Maksymalna iloœæ czarnych kwadratów(do losowania):" << black_numba_max << std::endl;

	srand(time(NULL));
	int black_square_numba = rand() % (black_numba_max - black_numba_min) + black_numba_min;
	std::cout << "Wylosowana iloœæ czarnych kwadratów(ostatecznie): " << black_square_numba << std::endl;

	return black_square_numba;
}
std::vector<int> prepare_black_squares(std::vector<std::vector<int>> table, int wk, int black_square_numba) {
	//Wstawianie czarnych kwadratów
	std::vector<int> coords_of_black_squares;
	coords_of_black_squares.resize(2 * black_square_numba);
	int x, y;
	int igrek = 0;
	int iks = 1;
	
	for (int counter = 0; counter < black_square_numba;) {
		y = rand() % wk;
		x = rand() % wk;

		if (table[y][x] != 7) {
			table[y][x] = 7;
			counter++;
			//wstawienie koordynatów czarnych kwadratów do vektora coords_of_black_squares
			coords_of_black_squares[igrek] = y;
			igrek = igrek + 2;
			coords_of_black_squares[iks] = x;
			iks = iks + 2;
		}
	}
	return coords_of_black_squares;
}

std::vector<std::vector<int>> draw_black_squares(std::vector<std::vector<int>> table, std::vector<int> coords_of_black_squares) {
	int y, x;
	int counter = 0;
	int igrek = 0;
	int iks = 1;
	while (counter < (coords_of_black_squares.size() / 2)) {
		y = coords_of_black_squares[igrek];
		x = coords_of_black_squares[iks];
		table[y][x] = 7;
		counter++;
		igrek += 2;
		iks += 2;
	}
	return table;
}


float checker(std::vector < std::vector<int>> a) {
	int max_score = size(a) * size(a);
	int current_score = max_score;
	float result;
	for (int b = 0; b < size(a); b++) {
		for (int c = 0; c < size(a); c++) {
			if (a[b][c] == 0) current_score = current_score - 1;
		}
	}
	result = (float)current_score / (float)max_score;
	return result;
}

std::vector<std::vector<int>> create_lamp_and_light_squares(std::vector < std::vector<int>> table, int wielkosc_kwadratowa) {
	//wstawienie lampek jako rozwiazanie
	for (int i = 0; i < wielkosc_kwadratowa; i++) {
		for (int j = 0; j < wielkosc_kwadratowa; j++) {
			//pocz¹tek funkcji tworz¹cej lampki ze œwiat³ami 
			if (table[i][j] != 7 && table[i][j] != 5 && table[i][j] != 6) {
				int y, x;
				table[i][j] = 5;
				if (j != 0) {//west light
					x = j - 1;
					y = i;
					while (table[y][x] != 7) {
						table[y][x] = 6;
						x--;
						if (x == -1) {
							break;
						}
					}
				}
				if (j != wielkosc_kwadratowa - 1) {//east light
					x = j + 1;
					y = i;
					while (table[y][x] != 7) {
						table[y][x] = 6;
						x++;
						if (x == wielkosc_kwadratowa) break;
					}
				}
				if (i != 0) { //north light
					x = j;
					y = i - 1;
					while (table[y][x] != 7) {
						table[y][x] = 6;
						y--;
						if (y == -1) break;
					}
				}
				if (i != wielkosc_kwadratowa - 1) {//south light
					x = j;
					y = i + 1;
					while (table[y][x] != 7) {
						table[y][x] = 6;
						y++;
						if (y == wielkosc_kwadratowa) break;
					}
				}

			}
			//koniec funkcji tworz¹cej lampki ze œwiat³ami
		}
	}
	return table;
}


std::vector<std::vector<int>> set_numbers_on_black_squares(std::vector< std::vector<int>> table, int wielkosc_kwadratowa, std::vector<int> coords_of_black_squares, int black_square_numba) {
	int min_random_numsquares = round(black_square_numba / 4);
	int max_random_numsquares = round(black_square_numba / 2);
	int how_much_numbersquares = rand() % (max_random_numsquares - min_random_numsquares) + min_random_numsquares;
	std::cout << "Maksymalna iloœæ czarnych kwadratów z numerkami(do losowania): " << max_random_numsquares << std::endl;
	std::cout << "Minimalna iloœæ czarnych kwadratów z numerkami(do losowania): " << min_random_numsquares << std::endl;
	std::cout << "Ostatecznie wylosowana iloœæ czarnych kwadratów z numerkami: " << how_much_numbersquares << std::endl;
	//ustawienie numerków na czarnych kwadratach
	int counter = 0;
	int igrek = 0;
	int iks = 1;
	for (counter; counter < how_much_numbersquares;) {
		int y = coords_of_black_squares[igrek];
		int x = coords_of_black_squares[iks];
		int yes = 0;
		if (x != 0) {// czy po lewej jest
			if (!(std::find(v.begin(), v.end(), table[y][x - 1]) != v.end())) yes++;
		}
		if (x != wielkosc_kwadratowa - 1) { //czy po prawej jest
			if (!(std::find(v.begin(), v.end(), table[y][x + 1]) != v.end())) yes++;
		}
		if (y != 0) {//czy u góry jest
			if (!(std::find(v.begin(), v.end(), table[y - 1][x]) != v.end())) yes++;
		}
		if (y != wielkosc_kwadratowa - 1) { //czy na dole jest
			if (!(std::find(v.begin(), v.end(), table[y + 1][x]) != v.end())) yes++;
		}
		if (yes != 0) {
			table[y][x] = yes;
			counter++;
		}
		igrek = igrek + 2;
		iks = iks + 2;
	}
	return table;
}
void draw_the_table(std::vector<std::vector<int>> table) {
	//rysuj ostateczn¹ tablicê
	for (auto r : table) {
		std::cout << "[ ";
		for (auto v : r) {
			if (v == 7) {
				std::cout << char(254) << " ";
				continue;
			}
			std::cout << v << " ";
		}
		std::cout << "]" << std::endl;
	}
}

void draw_the_coords_of_black_squares(std::vector<int> coords_of_black_squares) {
	//wypisz koordynaty czarnych kwadratów
	std::cout << std::endl;
	std::cout << "[ ";
	for (auto v : coords_of_black_squares) {
		std::cout << v << " ";
	}
	std::cout << "]" << std::endl;
}

int main(int argc, char* argv[]) {
	//wielkosc_tablicy 2D ma byæ wpisana przez u¿ytkownika
	std::cin >> wielkosc_kwadratowa;

	//deklaracja dynamicznej tablicy 2D
	std::vector<std::vector<int>> table;
	table.resize(wielkosc_kwadratowa); //table i wielkosc_kwadratowa
	for (int i = 0; i < wielkosc_kwadratowa; i++)
		table[i].resize(wielkosc_kwadratowa);
	
	int black_square_numba = numba_of_black_squares(wielkosc_kwadratowa);
	//zainicjowanie tablicy wektorowej i przypisanie jej wielkoœci adekwatnej 
	std::vector<int> coords_of_black_squares;
	coords_of_black_squares.resize(black_square_numba);
	//przygotowanie(a w³aœciwie wylosowanie) koordynatów i przechowanie jej w  tablicy wektorowej,
	// ¿eby wiedzieæ gdzie czarne kwadraty bêd¹ w œrodku tablicy wektorowej kwadratowej
	coords_of_black_squares = prepare_black_squares(table, wielkosc_kwadratowa, black_square_numba);
	//wypisanie wszystkich koordynatów wylosowanych randomowo w tablicy na standardowe wyjœcie
	draw_the_coords_of_black_squares(coords_of_black_squares);
	//zaktualizowanie tablicy o czarne kwadraty
	table = draw_black_squares(table, coords_of_black_squares);
	//zaktualizowanie tablicy o nowe lampki które produkuj¹ œwiat³o
	table = create_lamp_and_light_squares(table, wielkosc_kwadratowa);
	table = set_numbers_on_black_squares(table, wielkosc_kwadratowa, coords_of_black_squares, black_square_numba);
	draw_the_table(table);
}