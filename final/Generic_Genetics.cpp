#include<vector>
#include<random>
#include<functional>
#include<iostream>
#include <fstream>
#include "functions.h"
#include <math.h> 
#include <thread>
#include<mutex>
#include<chrono>

using chromosome_t = std::vector<std::vector<int>>;
functions func;
std::random_device device;
std::mt19937 generator(device());
/*
*   >0 = czarny kwadrat z 0
	>1 = czarny kwadrat z 1
	>2 = czarny kwadrat z 2
	>3 = czarny kwadrat z 3
	>4 = czarny kwadrat z 4
	>5 = lampka
	>6 = œwiat³o
	>7 = czarny kwadrat
	>8 = puste pole


*/
struct Member {
	chromosome_t DNA;
	double fitness = 0;
};
struct Population {
	std::vector <Member> Members = std::vector <Member>(20);
};
struct Archipelago {
	std::vector <Population> Islands = std::vector <Population> (5);
};

double fitness_check(d_vector table, std::vector<int> coords, int max) {
	int fitness_count = max;
	int wielkosc_kwadratowa = table.size();
	std::vector<int> vec = { 1,2,3,4 };
	for (int y = 0; y < table.size(); y++) {
		for (int x = 0; x < table.size(); x++) {
			if (table[y][x] == 8) {
				fitness_count--;
			}
		}
	}
	for (int j = 0; j < coords.size(); j = j + 3) {
		int y = coords[j];
		int x = coords[j + 1];
		if (coords[j + 2] == 0) {
			if (x != 0) {
				if (table[y][x - 1] == 5) {
					fitness_count--;
				}
			}
			//sprawdzenie czy po prawej stronie nie jesteœmy na koñcu planszy
			if (x != wielkosc_kwadratowa - 1) {
				if (table[y][x + 1] == 5) {
					fitness_count--;
				}
			}
			//sprawdzenie czy u góry nie jesteœmy na koñcy planszy
			if (y != 0) {
				if (table[y - 1][x] == 5) {
					fitness_count--;
				}
			}
			//sprawdzenie czy na dole nie jesteœmy na koñcu planszy
			if (y != wielkosc_kwadratowa - 1) {
				if (table[y + 1][x] == 5) {
					fitness_count--;
				}
			}
		}
		else if (std::find(vec.begin(), vec.end(), table[y][x]) != vec.end()) {
			int to_get = coords[j + 2];
			if (x != 0) {
				if (table[y][x - 1] == 5) {
					to_get--;
				}
			}
			//sprawdzenie czy po prawej stronie nie jesteœmy na koñcu planszy
			if (x != wielkosc_kwadratowa - 1) {
				if (table[y][x + 1] == 5) {
					to_get--;
				}
			}
			//sprawdzenie czy u góry nie jesteœmy na koñcy planszy
			if (y != 0) {
				if (table[y - 1][x] == 5) {
					to_get--;
				}
			}
			//sprawdzenie czy na dole nie jesteœmy na koñcu planszy
			if (y != wielkosc_kwadratowa - 1) {
				if (table[y + 1][x] == 5) {
					to_get--;
				}
			}
			if (to_get < 0) to_get = -to_get;
			fitness_count = fitness_count - to_get;
		}
	}
	double result = (float)fitness_count / (float)max;
	return result;
}


std::vector<Member> init_pop(chromosome_t table, std::vector<int> coords) {
	Population pop;
	srand((unsigned)time(0));

	for (int i = 0; i < pop.Members.size(); i++) {
		int max = func.get_fitness_max(table, coords);

		pop.Members.at(i).DNA = func.initialize_table(pop.Members.at(i).DNA);
		pop.Members.at(i).DNA = func.copy_d_vector(pop.Members.at(i).DNA, table);

		pop.Members.at(i).DNA = func.generate_member(pop.Members.at(i).DNA, coords);
		//func.draw_the_table(pop.Members.at(i).DNA);
		//std::cout << std::endl;
		pop.Members.at(i).fitness = fitness_check(pop.Members.at(i).DNA, coords, max);
		//std::cout << pop.Members.at(i).fitness << std::endl;
	}
	return pop.Members;
}


//returns member index
int roulette_selection(std::vector<Member> Members) {
	//Count Fitness_MAX
	double Fitness_MAX = 0.0;
	for (int i = 0; i < Members.size(); i++) {
		Fitness_MAX += Members.at(i).fitness;
	}

	//counting the individual normalized fitness values for each member
	std::vector<double> fit;
	for (int i = 0; i < Members.size(); i++) {
		double count_fit = (Members.at(i).fitness / Fitness_MAX);
		fit.push_back(count_fit);
	}

	//pog
	std::vector<double> accumulated_normalized_fitness_values;
	for (int i = 0; i < fit.size(); i++) {
		if (i == 0) {
			accumulated_normalized_fitness_values.push_back(fit[0]);
			//std::cout << accumulated_normalized_fitness_values[i] << std::endl;
			continue;
		}
		accumulated_normalized_fitness_values.push_back((fit[i] + accumulated_normalized_fitness_values[i - 1]));
		//std::cout << accumulated_normalized_fitness_values[i] << std::endl;
	}

	std::uniform_real_distribution<double> distribution(0, 1);
	double RANDOM_VALUE = distribution(generator);
	for (int i = 0; i < accumulated_normalized_fitness_values.size(); i++) {
		if (accumulated_normalized_fitness_values[i] > RANDOM_VALUE) return i;
	}
}

//returns member index
int tournament_selection(std::vector<Member> Members, int k) { //k = tournament size
	std::vector<int> tournament_members;
	int popsize = Members.size();
	std::uniform_int_distribution<int> distribution(0, popsize-1);
	for (int j = 0; j < k;) {
		int i = distribution(generator); //random index number of member
		if (!(std::find(tournament_members.begin(), tournament_members.end(), i) != tournament_members.end())) { //if not already participating
			tournament_members.push_back(i);
			++j;
		}
	}
	//now ive got indexes of members participating in tournament
	//lets fight
	int result = 0;
	double best_one = 0;
	for (int i = 0; i < tournament_members.size(); ++i) {
		if (Members.at(tournament_members[i]).fitness > best_one) {
			best_one = Members.at(tournament_members[i]).fitness;
			result = tournament_members[i];
		}
	}
	return result;
}


std::vector<Member> one_p_crossover_pop(std::vector<Member> Members, int selected, int selected2) {
	std::vector<Member> newMembers = Members;
	int max_i = Members.at(selected).DNA.size() * Members.at(selected).DNA.size() - 1;
	std::uniform_int_distribution<int> distribution(0, max_i);
	int i = distribution(generator);
	//std::cout << i << std::endl;
	for (int j = 0; j < i; ++j) {
		int y = i / Members.at(selected).DNA.size();
		int x = i % Members.at(selected).DNA.size();
		newMembers.at(selected).DNA[y][x] = Members.at(selected).DNA[y][x];
		newMembers.at(selected2).DNA[y][x] = Members.at(selected2).DNA[y][x];
	}

	for (i; i < Members.at(selected).DNA.size() * Members.at(selected).DNA.size(); ++i) { // from i (rand int 0-399) to 399
		int y = i / Members.at(selected).DNA.size();
		int x = i % Members.at(selected).DNA.size();
		newMembers.at(selected).DNA[y][x] = Members.at(selected2).DNA[y][x];
		newMembers.at(selected2).DNA[y][x] = Members.at(selected).DNA[y][x];
	}

	//fixing the table so that if the light doesnt shine on it the 6 is deleted
	for (int pog = 0; pog < Members.size(); ++pog) {
		newMembers.at(pog).DNA = func.fix_table(newMembers.at(pog).DNA);
		for (int y = 0; y < Members.at(pog).DNA.size(); ++y) {
			for (int x = 0; x < Members.at(pog).DNA.size(); ++x) {
				if (Members.at(pog).DNA[y][x] == 8) newMembers.at(pog).DNA = func.light_creation(newMembers.at(pog).DNA, y, x);
			}
		}
	}

	return newMembers;
}


std::vector<Member> two_p_crossover_pop(std::vector<Member> Members, int selected, int selected2) {
	std::vector<Member> newMembers = Members;
	int max_i = Members.at(selected).DNA.size() * Members.at(selected).DNA.size() - 1;

	std::uniform_int_distribution<int> distribution(0, max_i);
	int i = distribution(generator);
	//std::cout << "The i is " << i << std::endl;
	std::uniform_int_distribution<int> distribution2(i, max_i);
	int i02 = distribution2(generator);
	//std::cout << "The i02 is " << i02 << std::endl;

	//from beginning until i copy the values into the new container
	for (int j = 0; j < i; ++j) {
		int y = i / Members.at(selected).DNA.size();
		int x = i % Members.at(selected).DNA.size();
		newMembers.at(selected).DNA[y][x] = Members.at(selected).DNA[y][x];
		newMembers.at(selected2).DNA[y][x] = Members.at(selected2).DNA[y][x];
	}
	for (i; i < i02; ++i) { // from i (rand int 0-399) to 399
		int y = i / Members.at(selected).DNA.size();
		int x = i % Members.at(selected).DNA.size();
		newMembers.at(selected).DNA[y][x] = Members.at(selected2).DNA[y][x];
		newMembers.at(selected2).DNA[y][x] = Members.at(selected).DNA[y][x];
	}
	for (i02; i02 < Members.at(selected).DNA.size() * Members.at(selected).DNA.size(); ++i02) { // from i (rand int 0-399) to 399
		int y = i02 / Members.at(selected).DNA.size();
		int x = i02 % Members.at(selected).DNA.size();
		newMembers.at(selected).DNA[y][x] = Members.at(selected).DNA[y][x];
		newMembers.at(selected2).DNA[y][x] = Members.at(selected2).DNA[y][x];
	}

	//fixing the table so that if the light doesnt shine on it the 6 is deleted
	for (int pog = 0; pog < Members.size(); ++pog) {
		newMembers.at(pog).DNA = func.fix_table(newMembers.at(pog).DNA);
		// and then empty spaces are filled with lights
		for (int y = 0; y < Members.at(pog).DNA.size(); ++y) {
			for (int x = 0; x < Members.at(pog).DNA.size(); ++x) {
				if (Members.at(pog).DNA[y][x] == 8) newMembers.at(pog).DNA = func.light_creation(newMembers.at(pog).DNA, y, x);
			}
		}
	}
	return newMembers;
}


std::vector<Member> mutate_member(std::vector<Member> Members) {
	std::vector<int> light_coords;
	int counter = 0;
	std::uniform_int_distribution<int> distribution(0, (Members.size() - 1));
	int rand_member = distribution(generator);
	//int members_size = Members.size();
	//int rand_member = rand() % members_size;
	while (counter != 2) {
		int MAX = Members.at(0).DNA.size();
		std::uniform_int_distribution<int> ydist(0, Members.at(0).DNA.size()-1);
		int y = ydist(generator);
		std::uniform_int_distribution<int> xdist(0, Members.at(0).DNA.size() - 1);
		int x = xdist(generator);
		if (Members.at(rand_member).DNA[y][x] == 6 || Members.at(rand_member).DNA[y][x] == 8) {
			Members.at(rand_member).DNA = func.light_creation(Members.at(rand_member).DNA, y, x);
			++counter;
		}
	}
	return Members;
}


bool should_endv1(std::vector<Population> Generations, int limit) {
	std::cout << "Wielkosc Generacji:" << Generations.size() << std::endl;
	if (Generations.size() >= limit) return true;
	else return false;
}


bool should_endv2(std::vector<Member> Members) {
	double all_fitnesses = 0;
	for (int i = 0; i < Members.size(); ++i) {
		all_fitnesses += Members.at(i).fitness;
	}
	std::cout << "The average fitness is: " << (double)all_fitnesses / Members.size() << std::endl;
	if (((double)all_fitnesses / Members.size()) >= 0.90) return true;
	else return false;
}


bool should_endv3(std::vector<Member> Members, double to_end) {
	double all_fitnesses = 0;

	for (int i = 0; i < Members.size(); ++i) {
		all_fitnesses += Members.at(i).fitness;
	}
	//std::cout << "All fitnesses equals to " << all_fitnesses << std::endl;
	double average = all_fitnesses / Members.size();
	//std::cout << "Average equals to " << average << std::endl;
	double variance = 0;
	for (int i = 0; i < Members.size(); ++i) {
		variance += pow(Members.at(i).fitness - average, 2);
	}
	//std::cout << "Variance equals to " << variance << std::endl;
	double standard_deviation = sqrt(variance);
	//std::cout << "Standard deviation equals to " << standard_deviation << std::endl;
	if (standard_deviation < to_end) return true;
	return false;
}


chromosome_t return_best_member(std::vector<Member> Members) {
	double max = 0;
	int pog = 0;
	for (int i = 0; i < Members.size(); ++i) {
		if (Members.at(i).fitness > max) {
			max = Members.at(i).fitness;
			pog = i;
		}
	}
	std::cout << Members.at(pog).fitness << std::endl;
	return Members.at(pog).DNA;
}


std::vector<int> get_numbers_f() {
	std::vector<int> numbers;
	std::ifstream in("abc.txt", std::ios::in);
	int number;
	while (in >> number) numbers.push_back(number);
	in.close();
	return numbers;
}


chromosome_t input_table(chromosome_t table, std::vector<int> numbers) {
	int wye = 0;
	int ex = 1;
	int y, x;
	for (int i = 0; i < numbers.size(); i++) {
		if (i % 3 == 0) y = numbers[i];
		if (i % 3 == 1) x = numbers[i];
		if (i % 3 == 2) table[y][x] = numbers[i];
	}
	return table;
}


std::mutex myMutex;
//Generations.at(i).Islands.at(isl_numb).Members
void do_it(int w_isl, std::vector <Archipelago>& Generations, int w_gen, int w_selection, int w_crossover, double w_prob_mut, double w_prob_cross, int max, std::vector<int> file_input) {
	std::vector<int> selected(Generations.at(w_gen).Islands.at(w_isl).Members.size());
	std::vector<Member> newMembers = Generations.at(w_gen).Islands.at(w_isl).Members;
	
	switch (w_selection) {
	case 0:
		for (int j = 0; j < Generations.at(w_gen).Islands.at(w_isl).Members.size(); ++j) {
			selected[j] = roulette_selection(Generations.at(w_gen).Islands.at(w_isl).Members);
		}
		break;
	case 1:
		for (int j = 0; j < Generations.at(w_gen).Islands.at(w_isl).Members.size(); ++j) {
			selected[j] = tournament_selection(Generations.at(w_gen).Islands.at(w_isl).Members, 5);
		}
		break;

	}

	switch (w_crossover) {
	case 0:
	{
		std::uniform_real_distribution<double> distribution_cross(0, 1);
		for (int j = 0; j < selected.size(); j = j + 2) {
			double rand_number_cross = distribution_cross(generator);
			if (rand_number_cross > (1 - w_prob_cross)) {
				newMembers = one_p_crossover_pop(Generations.at(w_gen).Islands.at(w_isl).Members, selected[j], selected[j + 1]);
			}
		}

	}
	break;
	case 1:
	{
		std::uniform_real_distribution<double> distribution_cross(0, 1);
		for (int j = 0; j < selected.size(); j = j + 2) {
			double rand_number_cross = distribution_cross(generator);
			if (rand_number_cross > (1 - w_prob_cross)) {
				newMembers = two_p_crossover_pop(Generations.at(w_gen).Islands.at(w_isl).Members, selected[j], selected[j + 1]);
			}
		}
	}
	break;
	}

	//mutacja
	std::uniform_real_distribution<double> distribution(0, 1);
	double rand_number_mut = distribution(generator);
	//std::cout << "Random number is " << rand_number_mut << std::endl;
	if (rand_number_mut > (1 - w_prob_mut)) {
		newMembers = mutate_member(newMembers);
	}
	//ustawienie ich nowego fitnessu, w zale¿noœci od tego co potrafi¹
	for (int j = 0; j < Generations.at(w_gen).Islands.at(w_isl).Members.size(); ++j) {
		//int max = func.get_fitness_max(table, file_input);
		Generations.at(w_gen).Islands.at(w_isl).Members.at(j).fitness = fitness_check(Generations.at(w_gen).Islands.at(w_isl).Members.at(j).DNA, file_input, max);
	}
	Generations.at(w_gen).Islands.at(w_isl).Members = newMembers;
}


int main() {
	auto start = std::chrono::system_clock::now();
	//rysowanie tablicy wk*wk (kwadratowej)
	int wk = 20;
	chromosome_t table;
	std::vector<int> file_input;
	table = func.initialize_table(table);
	file_input = get_numbers_f(); //dumps all numbers from file into vector
	table = input_table(table, file_input); //modifies the table according to the vector
	func.draw_the_table(table);
	/******************************************************************************************************************************/
	std::vector<Archipelago> Generations; //now I have the table of all generations
	Generations.resize(1); //It has size 1

	//Initialization of 5 Islands
	for (int i = 0; i < Generations.at(0).Islands.size(); ++i) {
		//std::cout << Generations.at(0).Islands.size();
		Generations.at(0).Islands.at(i).Members = init_pop(table, file_input);
	}

	for (int i = 1; i < 5; ++i) {
		Generations.resize(i+1);
		Generations.at(i).Islands = Generations.at(i-1).Islands;//copy last generation for new selection and crossover
		//implementing exchange every 3 iterations
		if (i % 3 == 0) {
			std::uniform_int_distribution<> distrib(0, Generations.at(i).Islands.at(0).Members.size()-1); //random member from population
			for (int j = 0; j < Generations.at(0).Islands.size(); ++j) {
				for (int k = j+1; k < Generations.at(0).Islands.size(); ++k) {
					std::vector<int> the_colonists;
					//get ALWAYS 5 different members to exchange
					while (the_colonists.size() != 5) {
						int rand_member = distrib(generator);
						if (!(std::find(the_colonists.begin(), the_colonists.end(), rand_member) != the_colonists.end())) {
							the_colonists.push_back(rand_member);
							std::cout << "The random member to exchange between " << j << " Island and " << k << " Island is " << rand_member << std::endl;
						}
					}
					//exchange 5 colonists between two islands
					for (int l = 0; l < the_colonists.size(); l++) {
						Generations.at(i).Islands.at(j).Members.at(the_colonists[l]).DNA = Generations.at(i - 1).Islands.at(k).Members.at(the_colonists[l]).DNA;
						Generations.at(i).Islands.at(k).Members.at(the_colonists[l]).DNA = Generations.at(i - 1).Islands.at(j).Members.at(the_colonists[l]).DNA;
					}
				}
			}
		}
		std::cout << "Starting.." << std::endl;
		std::vector<std::thread> threads;//vector threads of size of Islands
		for (int isl_numb = 0; isl_numb < Generations.at(i).Islands.size(); ++isl_numb) {
			threads.emplace_back(do_it, isl_numb, std::ref(Generations), i, 0, 0, 0.15, 1.0, func.get_fitness_max(table, file_input), file_input);
		}
		for (std::thread  &temp: threads) {
			temp.join();
		}
		std::cout << "Done." << std::endl;
		should_endv2(Generations.at(i).Islands.at(0).Members);
	}
	std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
	std::cout << "Time for genetic algorithm: " << dur.count() << " seconds" << std::endl;
}
