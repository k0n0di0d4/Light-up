#include<vector>
typedef std::vector<std::vector<int>> d_vector;
typedef std::vector<int> s_vector;
class functions {
public:
	d_vector light_creation(d_vector table, int i, int j);
	d_vector initialize_table(d_vector table);
	void draw_the_table(d_vector table);
	d_vector copy_d_vector(d_vector table, d_vector t_be_copied);
	d_vector fill_blank_spaces(d_vector table);
	bool table_blank(d_vector table);
	s_vector copy_s_vector(s_vector table, s_vector t_be_copied);
	d_vector generate_member(d_vector table, s_vector coords);
	d_vector r_generate_member(d_vector table);
	int get_fitness_max(d_vector table, s_vector coords);
	d_vector fix_table(d_vector table);
	bool check_if_light(d_vector table, int i, int j);
};

