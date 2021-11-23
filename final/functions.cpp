#include<vector>
#include "functions.h"
#include <iostream>
#include <random>

typedef std::vector<std::vector<int>> d_vector;
typedef std::vector<int> s_vector;


s_vector v = { 0, 1, 2, 3, 4, 7 };


d_vector functions::light_creation(d_vector table, int i, int j) {
	int wielkosc_kwadratowa = table.size();
	int y, x;
	if (check_if_light(table, i, j) == true) return table;
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


void functions::draw_the_table(d_vector table) {
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


//funkcja która inicjuje tablicê 20x20 i ustawia wszystkie wartoœci na 8
d_vector functions::initialize_table(d_vector table) {
	int wk = 20;
	table.resize(wk);
	for (int i = 0; i < wk; i++) {
		table[i].resize(wk);
	}
	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table.size(); j++) {
			table[i][j] = 8;
		}
	}
	return table;
}


d_vector functions::copy_d_vector(d_vector table, d_vector t_be_copied) {
	for (int i = 0; i < t_be_copied.size(); i++) {
		for (int j = 0; j < t_be_copied.size(); j++) {
			table[i][j] = t_be_copied[i][j];
		}
	}
	return table;
}


s_vector functions::copy_s_vector(s_vector table, s_vector t_be_copied) {
	table.resize(t_be_copied.size());
	for (int i = 0; i < t_be_copied.size(); i++) {
		table[i] = t_be_copied[i];
	}
	return table;
}


d_vector functions::fill_blank_spaces(d_vector table) {
	int wielkosc_kwadratowa = table.size();
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<int> distribution(0, 9);
	//wstawienie lampek jako rozwiazanie
	int h = 0;
	while (table_blank(table) != true) {
		int i = distribution(generator);
		int j = distribution(generator);
		h++;
		if (table[i][j] == 8) {
			table = light_creation(table, i, j);
		}
	}
	std::cout << h << std::endl;
	return table;
}


bool functions::table_blank(d_vector table) {
	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table.size(); j++) {
			if (table[i][j] == 8) return false;
		}
	}
	return true;
}

d_vector functions::generate_member(d_vector table, s_vector coords_p) {
	s_vector coords = copy_s_vector(coords, coords_p);
	int wielkosc_kwadratowa = table.size();
	while (coords.size() != 0) {
		int MAX = coords.size() / 3; //size divided by 3
		int j = (rand() % MAX) * 3; //only numbers divisible by 3
		int x, y, max;
		y = coords[j];
		x = coords[j+1];
		if (table[y][x] != 7 && table[y][x] != 0) {
			max = table[y][x];
			int count = 0;
			//sprawdzenie czy po lewej stronie nie jesteœmy na koñcu planszy
			if (x != 0) {
				if (table[y][x - 1] == 8) {
					table = light_creation(table, y, x - 1);
					count++;
					if (count == max) {
						continue;
					}
				}
			}
			//sprawdzenie czy po prawej stronie nie jesteœmy na koñcu planszy
			if (x != wielkosc_kwadratowa - 1) {
				if (table[y][x + 1] == 8) {
					table = light_creation(table, y, x + 1);
					count++;
					if (count == max) {
						continue;
					}
				}
			}
			//sprawdzenie czy u góry nie jesteœmy na koñcy planszy
			if (y != 0) {
				if (table[y - 1][x] == 8) {
					table = light_creation(table, y - 1, x);
					count++;
					if (count == max) {
						continue;
					}
				}
			}
			//sprawdzenie czy na dole nie jesteœmy na koñcu planszy
			if (y != wielkosc_kwadratowa - 1) {
				if (table[y + 1][x] == 8) {
					table = light_creation(table, y + 1, x);
					count++;
					if (count == max) {
						continue;
					}
				}
			}
		}
		else if (table[y][x] == 7 && table[y][x] != 0) {
			//sprawdzenie czy po lewej stronie nie jesteœmy na koñcu planszy
			if (x != 0) {
				if (table[y][x - 1] == 8) {
					table = light_creation(table, y, x - 1);
				}
			}
			//sprawdzenie czy po prawej stronie nie jesteœmy na koñcu planszy
			if (x != wielkosc_kwadratowa - 1) {
				if (table[y][x + 1] == 8) {
					table = light_creation(table, y, x + 1);
				}
			}
			//sprawdzenie czy u góry nie jesteœmy na koñcy planszy
			if (y != 0) {
				if (table[y - 1][x] == 8) {
					table = light_creation(table, y - 1, x);
				}
			}
			//sprawdzenie czy na dole nie jesteœmy na koñcu planszy
			if (y != wielkosc_kwadratowa - 1) {
				if (table[y + 1][x] == 8) {
					table = light_creation(table, y + 1, x);
				}
			}
		}

		coords.erase(coords.begin() + j, coords.begin() + j + 3);
	}
		for (int y = 0; y < table.size(); y++) {
			for (int x = 0; x < table.size(); x++) {
				if (table[y][x] == 8) table = light_creation(table, y, x);
			}
		}
	return table;
}


int functions::get_fitness_max(d_vector table, s_vector coords) {
	std::vector<int> vec = {1,2,3,4};
	int wielkosc_kwadratowa = table.size();
	int f_max0 = (table.size() * table.size()) - (coords.size() / 3);
	int f_max1 = 0;
	
	for (int j = 0; j < coords.size(); j = j + 3) {
		int y = coords[j];
		int x = coords[j + 1];
		if (table[y][x] == 0) {
			if (x != 0) {
				if (table[y][x - 1] == 8) {
					f_max1++;
				}
			}
			//sprawdzenie czy po prawej stronie nie jesteœmy na koñcu planszy
			if (x != wielkosc_kwadratowa - 1) {
				if (table[y][x + 1] == 8) {
					f_max1++;
				}
			}
			//sprawdzenie czy u góry nie jesteœmy na koñcy planszy
			if (y != 0) {
				if (table[y - 1][x] == 8) {
					f_max1++;
				}
			}
			//sprawdzenie czy na dole nie jesteœmy na koñcu planszy
			if (y != wielkosc_kwadratowa - 1) {
				if (table[y + 1][x] == 8) {
					f_max1++;
				}
			}
		}
		else if (std::find(vec.begin(), vec.end(), table[y][x]) != vec.end()) {
			f_max1 += coords[j + 2];
		}
	}
	int max = f_max0 + f_max1;
	return max;
}


d_vector functions::fix_table(d_vector table) {
	int wk = table.size();
	for (int i = 0; i < wk; ++i) {
		for (int j = 0; j < wk; ++j) {
			int y = i;
			int x = j;
			if (table[y][x] == 6) {
				//sprawdzenie czy po lewej stronie nie jesteœmy na koñcu planszy
				if (x != 0) {
					//dopóki nie napotka jakiœ czarny kwadracik to pêtla dzia³a
					while (!(std::find(v.begin(), v.end(), table[y][x]) != v.end())) {
						if (table[y][x] == 5) break; //koniec przy znalezieniu lampki
						else if (x == 0) break;
						--x; //leci w lewo
					}
					if (table[y][x] == 5) continue;
				}
				x = j;
				//sprawdzenie czy po prawej stronie nie jesteœmy na koñcu planszy
				if (x != wk - 1) {
					while (!(std::find(v.begin(), v.end(), table[y][x]) != v.end())) {
						if (table[y][x] == 5) break; //koniec przy znalezieniu lampki
						else if (x == wk - 1) break;
						++x; //leci w lewo
					}
					if (table[y][x] == 5) continue;
				}
				//sprawdzenie czy u góry nie jesteœmy na koñcy planszy
				x = j;
				if (y != 0) {
					while (!(std::find(v.begin(), v.end(), table[y][x]) != v.end())) {
						if (table[y][x] == 5) break; //koniec przy znalezieniu lampki
						else if (y == 0) break;
						--y; //leci w górê
					}
					if (table[y][x] == 5) continue;
				}
				y = i;
				//sprawdzenie czy na dole nie jesteœmy na koñcu planszy
				if (y != wk - 1) {
					while (!(std::find(v.begin(), v.end(), table[y][x]) != v.end())) {
						if (table[y][x] == 5) break; //koniec przy znalezieniu lampki
						else if (y == wk - 1) break;
						++y; //leci w dó³
					}
					if (table[y][x] == 5) continue;
				}
				y = i;
				x = j;
				table[y][x] = 8;
			}
		}
	}
	return table;
}


d_vector functions::r_generate_member(d_vector table) {
	
	return table;
}


bool functions::check_if_light(d_vector table, int i, int j) {
	int y = i;
	int x = j;
	int wk = table.size();
	if (x != 0) {
		int x = j - 1;
		while (!(std::find(v.begin(), v.end(), table[y][x]) != v.end())) {
			if (table[y][x] == 5) break;
			if (x == 0) break;
			--x;
		}
		if (table[y][x] == 5) return true;
	}
	y = i;
	x = j;
	if (x != (wk - 1)) {
		x = j + 1;
		while (!(std::find(v.begin(), v.end(), table[y][x]) != v.end())) {
			if (table[y][x] == 5) break;
			if (x == (wk - 1)) break;
			++x;
		}
		if (table[y][x] == 5) return true;
	}
	y = i;
	x = j;
	if (y != 0) {
		y = i - 1;
		while (!(std::find(v.begin(), v.end(), table[y][x]) != v.end())) {
			if (table[y][x] == 5) break;
			if (y == 0) break;
			--y;
		}
		if (table[y][x] == 5) return true;
	}
	y = i;
	x = j;
	if (y != (wk - 1)) {
		y = i + 1;
		while (!(std::find(v.begin(), v.end(), table[y][x]) != v.end())) {
			if (table[y][x] == 5) break;
			if (y == (wk - 1)) break;
			++y;
		}
		if (table[y][x] == 5) return true;
	}
	return false;
}