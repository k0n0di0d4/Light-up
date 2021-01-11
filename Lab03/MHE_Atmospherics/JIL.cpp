#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <string>

typedef std::vector<int> s_vector;
typedef std::vector<s_vector> d_vector;
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

int wielkosc_kwadratowa;
s_vector v = { 1, 2, 3, 4, 7 };

//funkcja ustalaj¹ca ile czarnych kwadratów bêdzie w tablicy kwadratowej
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

//funkcja przygotowuj¹ca koordynaty czarnych kwadratów gdzie trzeba je bêdzie wstawiæ do tablicy kwadratowej
s_vector prepare_black_squares(d_vector table, int black_square_numba) {
	int wk = table.size();
	//Wstawianie czarnych kwadratów
	s_vector coords_of_black_squares;
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

// funkcja wstawiaj¹ca czarne kwadraty do tablicy kwadratowej na podstawie wybranych wczeœniej pozycji 
d_vector draw_black_squares(d_vector table, s_vector coords_of_black_squares) {
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

//funkcja celu, oceniaj¹ca nasze zadanie wed³ug kryteriów opisanych przeze mnie
float checker(d_vector a, s_vector coords_of_black_squares) {
	int wk = a.size();
	int max_score = size(a) * size(a);
	int current_score = max_score;
	float result;
	for (int b = 0; b < size(a); b++) {
		for (int c = 0; c < size(a); c++) {
			if (a[b][c] == 0) current_score = current_score - 1;
		}
	}
	int igrek = 0;
	int iks = 1;
	int y, x;
	for (int i = 0; i < coords_of_black_squares.size() / 2; i++) {
		y = coords_of_black_squares[igrek];
		x = coords_of_black_squares[iks];
		if (a[y][x] == 1 || a[y][x] == 2 || a[y][x] == 3 || a[y][x] == 4) {
			int beksa = a[y][x];
			if(y != 0){
				if (a[y - 1][x] == 5) beksa--;
			}
			if (y != wk - 1) {
				if (a[y + 1][x] == 5) beksa--;
			}
			if (x != 0) {
				if (a[y][x - 1] == 5) beksa--;
			}
			if (x != wk - 1) {
				if (a[y][x + 1] == 5) beksa--;
			}
			
			if (beksa < 0) beksa = 0;
			current_score = current_score - beksa;
		}
		igrek += 2;
		iks += 2;
	}
	result = (float)current_score / (float)max_score;
	return result;
}
//funkcja która tworzy lampke w danym punkcie i œwiat³o i zapisuje je do tablicy
d_vector light_creation(d_vector table, int i, int j) {
	int wielkosc_kwadratowa = table.size();
	int y, x;
	table[i][j] = 5;
	if (j != 0) {//west light
		x = j - 1;
		y = i;
		while (!(std::find(v.begin(), v.end(), table[y][x]) != v.end())) {
			table[y][x] = 6;
			x--;
			if (x == -1) break;
		}
	}
	if (j != wielkosc_kwadratowa - 1) {//east light
		x = j + 1;
		y = i;
		while (!(std::find(v.begin(), v.end(), table[y][x]) != v.end())) {
			table[y][x] = 6;
			x++;
			if (x == wielkosc_kwadratowa) break;
		}
	}
	if (i != 0) { //north light
		x = j;
		y = i - 1;
		while (!(std::find(v.begin(), v.end(), table[y][x]) != v.end())) {
			table[y][x] = 6;
			y--;
			if (y == -1) break;
		}
	}
	if (i != wielkosc_kwadratowa - 1) {//south light
		x = j;
		y = i + 1;
		while (!(std::find(v.begin(), v.end(), table[y][x]) != v.end())) {
			table[y][x] = 6;
			y++;
			if (y == wielkosc_kwadratowa) break;
		}
	}
	return table;
}

//funkcja przechodz¹ca wzd³u¿ wersów tablicy i tworz¹ca lampki tam gdzie zobaczy 0
d_vector fill_blank_spaces(d_vector table) {
	int wielkosc_kwadratowa = table.size();
	//wstawienie lampek jako rozwiazanie
	for (int i = 0; i < wielkosc_kwadratowa; i++) {
		for (int j = 0; j < wielkosc_kwadratowa; j++) {
			//pocz¹tek funkcji tworz¹cej lampki ze œwiat³ami 
			if (table[i][j] == 0) {
				table = light_creation(table, i, j);
			}
		}
	}
	return table;
}

//funkcja która na podstawie lokalizacji wpisanych ju¿ czarnych kwadratów wybiera niektóre z nich i ustawia na nich numerki
d_vector set_numbers_on_black_squares(std::vector< s_vector> table, s_vector coords_of_black_squares, int black_square_numba) {
	int wielkosc_kwadratowa = table.size();
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

//funkcja tworz¹ca lampki wokó³ czarnych kwadratów
d_vector generate_solution(d_vector table, s_vector coords_of_black_squares) {
	int wielkosc_kwadratowa = table.size();
	int counter, igrek, iks, x, y;
	counter = 0;
	igrek = 0;
	iks = 1;
	int max;
	//przejœcie po ca³ej tablicy(coords_of_black_squares), ¿eby wpisaæ dooko³a lampki
	while (counter < (coords_of_black_squares.size() / 2)) {
		y = coords_of_black_squares[igrek];
		x = coords_of_black_squares[iks];
		if (table[y][x] != 7) {
			max = table[y][x];
			int count = 0;
			//sprawdzenie czy po lewej stronie nie jesteœmy na koñcu planszy
			if (x != 0) {
				if (table[y][x - 1] == 0) {
					table = light_creation(table, y, x - 1);
					count++;
					if (count == max) {
						igrek += 2;
						iks += 2;
						counter++;
						continue;
					}
				}
			}
			//sprawdzenie czy po prawej stronie nie jesteœmy na koñcu planszy
			if (x != wielkosc_kwadratowa - 1) {
				if (table[y][x + 1] == 0) {
					table = light_creation(table, y, x + 1);
					count++;
					if (count == max) {
						igrek += 2;
						iks += 2;
						counter++;
						continue;
					}
				}
			}
			//sprawdzenie czy u góry nie jesteœmy na koñcy planszy
			if (y != 0) {
				if (table[y - 1][x] == 0) {
					table = light_creation(table, y - 1, x);
					count++;
					if (count == max) {
						igrek += 2;
						iks += 2;
						counter++;
						continue;
					}
				}
			}
			//sprawdzenie czy na dole nie jesteœmy na koñcu planszy
			if (y != wielkosc_kwadratowa - 1) {
				if (table[y + 1][x] == 0) {
					table = light_creation(table, y + 1, x);
					count++;
					if (count == max) {
						igrek += 2;
						iks += 2;
						counter++;
						continue;
					}
				}
			}

		}
		counter++;
		igrek += 2;
		iks += 2;
	}
	
	igrek = 0;
	iks = 1;
	counter = 0;
	while (counter < (coords_of_black_squares.size() / 2)) {
		y = coords_of_black_squares[igrek];
		x = coords_of_black_squares[iks];
		if (table[y][x] == 7) {
			//sprawdzenie czy po lewej stronie nie jesteœmy na koñcu planszy
			if (x != 0) {
				if (table[y][x - 1] == 0) {
					table = light_creation(table, y, x - 1);
				}
			}
			//sprawdzenie czy po prawej stronie nie jesteœmy na koñcu planszy
			if (x != wielkosc_kwadratowa - 1) {
				if (table[y][x + 1] == 0) {
					table = light_creation(table, y, x + 1);
				}
			}
			//sprawdzenie czy u góry nie jesteœmy na koñcy planszy
			if (y != 0) {
				if (table[y - 1][x] == 0) {
					table = light_creation(table, y - 1, x);
				}
			}
			//sprawdzenie czy na dole nie jesteœmy na koñcu planszy
			if (y != wielkosc_kwadratowa - 1) {
				if (table[y + 1][x] == 0) {
					table = light_creation(table, y + 1, x);
				}
			}
		}
		counter++;
		igrek += 2;
		iks += 2;
	}
	return table;
}

d_vector next_solution(d_vector table) {
	return table;
}

//rysowanie tablicy kwadratowej do standard output
void draw_the_table(d_vector table) {
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

//rysowanie tablicy do standard output
void output_the_coords_of_black_squares(s_vector coords_of_black_squares) {
	//wypisz koordynaty czarnych kwadratów
	std::cout << std::endl;
	std::cout << "[ ";
	for (auto v : coords_of_black_squares) {
		std::cout << v << " ";
	}
	std::cout << "]" << std::endl;
}

d_vector input_task(int wk) {
	d_vector table;
	table.resize(wk); //table i wielkosc_kwadratowa
	for (int i = 0; i < wk; i++)
		table[i].resize(wk);
	std::string str_input0, str_input1, str_input_numba;
	int int_input0, int_input1, int_input_numba;
	for (int i = 0;i < table.size() * table.size();i++){
		std::cin >> str_input0;
		std::cin >> str_input1;
		std::cin >> str_input_numba;
		if (str_input0 == "pog")break;
		int_input0 = std::stoi(str_input0);
		int_input1 = std::stoi(str_input1);
		int_input_numba = std::stoi(str_input_numba);
		table[int_input0][int_input1] = int_input_numba;
	}

	return table;
}


int main(int argc, char* argv[]) {
	//wielkosc_tablicy 2D ma byæ wpisana przez u¿ytkownika
	std::cin >> wielkosc_kwadratowa;

	//deklaracja dynamicznej tablicy 2D
	d_vector table;
	table.resize(wielkosc_kwadratowa); //table i wielkosc_kwadratowa
	for (int i = 0; i < wielkosc_kwadratowa; i++)
		table[i].resize(wielkosc_kwadratowa);
	
	int black_square_numba = numba_of_black_squares(wielkosc_kwadratowa);
	//zainicjowanie tablicy wektorowej i przypisanie do niej adekwatnej wielkoœci 
	s_vector coords_of_black_squares;
	coords_of_black_squares.resize(black_square_numba);
	//przygotowanie(a w³aœciwie wylosowanie) koordynatów i przechowanie jej w  tablicy wektorowej,
	// ¿eby wiedzieæ gdzie czarne kwadraty bêd¹ w œrodku tablicy wektorowej kwadratowej
	coords_of_black_squares = prepare_black_squares(table, black_square_numba);
	//wypisanie wszystkich koordynatów wylosowanych randomowo w tablicy na standardowe wyjœcie
	output_the_coords_of_black_squares(coords_of_black_squares);
	//zaktualizowanie tablicy o czarne kwadraty
	table = draw_black_squares(table, coords_of_black_squares);
	//ustawienie numerków na niektórych czarnych kwadratach 
	table = set_numbers_on_black_squares(table, coords_of_black_squares, black_square_numba);
	//zaktualizowanie tablicy o nowe lampki które produkuj¹ œwiat³o(v2 sprawia ¿e s¹ one utworzone wzglêdem kwadratów z numerkami)
	table = generate_solution(table, coords_of_black_squares);
	table = fill_blank_spaces(table);
	draw_the_table(table);
	std::cout << checker(table, coords_of_black_squares)*100 << "% / 100%" << std::endl;

	//#--------------------------------------#------------------------------------------------------------#----------------------------------------#-------------------------------#
	wielkosc_kwadratowa = 10;
	d_vector table02;
	s_vector coords_of_black_squares02 = {0, 0, 0, 9, 1, 2, 1, 3, 2, 2, 2, 5, 2, 7, 2, 8, 3, 3, 3, 6, 3, 8, 4, 2, 5, 7, 6, 1, 6, 3, 6, 6, 7, 1, 7, 2, 7, 4, 7, 7, 8, 6, 8, 7, 9, 0, 9, 9};
	output_the_coords_of_black_squares(coords_of_black_squares02);
	table02.resize(wielkosc_kwadratowa); //table i wielkosc_kwadratowa
	for (int i = 0; i < wielkosc_kwadratowa; i++)
		table02[i].resize(wielkosc_kwadratowa);
	
	table02 = draw_black_squares(table02, coords_of_black_squares02);
	table02[7][1] = 1; table02[8][7] = 1; table02[1][2] = 2; table02[3][8] = 2; table02[2][7] = 3; table02[7][7] = 3; table02[3][3] = 4; table02[6][6] = 4;
	table02 = generate_solution(table02, coords_of_black_squares02);
	draw_the_table(table02);
	std::cout << checker(table02, coords_of_black_squares02) * 100 << "% / 100%" << std::endl;
	//#--------------------------------------#------------------------------------------------------------#----------------------------------------#-------------------------------#
	d_vector table03;
	wielkosc_kwadratowa = 10;
	table03.resize(wielkosc_kwadratowa); //table i wielkosc_kwadratowa
	for (int i = 0; i < wielkosc_kwadratowa; i++)
		table03[i].resize(wielkosc_kwadratowa);
	table03 = input_task(10);
	draw_the_table(table03);
}