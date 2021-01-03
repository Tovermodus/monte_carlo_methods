//
// Created by tovermodus on 12/2/20.
//

#include <sstream>
#include "Medium.h"
Medium::Medium (const MediumParameters& params,std::mt19937 & rng):parameters(params)

{
	initialize_cells();
	initialize_rods(rng);
}
void Medium::initialize_rods(std::mt19937 & rng)
{
	int iterations = 0;
	int total_number_of_rods = (int)(parameters.rods_per_volume*parameters.width*parameters.height);
	int rod_number = 0;
	while(rod_number < total_number_of_rods && iterations < parameters.rod_placing_max_iterations)
	{
		std::cout << rod_number << " " << total_number_of_rods<<"\n";
		std::shared_ptr<Rod> new_rod = create_random_rod(rng);
		std::shared_ptr<Cell> cell = get_cell_of_rod(new_rod);
		cell->add_rod(new_rod);
		if(!rod_is_acceptable(new_rod)) {
			cell->remove_rod(new_rod);
		}
		else{
			rod_number++;
		}
		iterations ++;
	}
}
double Medium::calculate_energy() const
{
	double ret = 0;
	double rod_mass_difference = parameters.rod_length * parameters.rod_width * parameters.rod_width *
				     (parameters.rod_density - parameters.density);
	for (const std::shared_ptr<Cell> &cell : cells) {
		int n_rods = cell->number_rods_in_patch();
		for (int i = 0; i < cell->number_rods_in_cell(); ++i) {
			std::shared_ptr<Rod> r = cell->get_rod_in_cell(i);
			ret += r->get_y() * parameters.gravity*rod_mass_difference;
			if (r->get_y() - std::abs(std::sin(r->get_angle()) * parameters.rod_length) < 0)
				return 1e100;
			if (r->get_x() - std::abs(std::cos(r->get_angle()) * parameters.rod_length) < 0)
				return 1e100;
			if (r->get_x() + std::abs(std::cos(r->get_angle()) * parameters.rod_length) > parameters.width)
				return 1e100;
			if (r->get_y() > parameters.height)
				return 1e100;
			for (int j = 0; j < n_rods; ++j) {
				if (r == cell->get_rod_in_patch(j))
					continue;
				if (r->check_collision(cell->get_rod_in_patch(j))) {
					return 1e100;
				}
			}
		}
	}
	return ret;
}
bool Medium::rod_is_acceptable(const std::shared_ptr<Rod>& rod) const
{
	if(rod->get_y() - std::abs(std::sin(rod->get_angle())*parameters.rod_length)< 0)
		return false;
	if(rod->get_x() - std::abs(std::cos(rod->get_angle())*parameters.rod_length)< 0)
		return false;
	if(rod->get_x() + std::abs(std::cos(rod->get_angle())*parameters.rod_length)> parameters.width)
		return false;
	if(rod->get_y() > parameters.height)
		return false;
	for (int i = 0; i < rod->get_cell()->number_rods_in_patch(); i++) {
		if(rod->get_cell()->get_rod_in_patch(i) == rod)
			continue;
		if (rod->get_cell()->get_rod_in_patch(i)->check_collision(rod)) {
			return false;
		}
	}
	return true;
}
std::shared_ptr<Rod> Medium::create_random_rod(std::mt19937 & rng) const
{
	std::uniform_real_distribution<> x_distrib(0, parameters.width);
	std::uniform_real_distribution<> y_distrib(0, parameters.height);
	std::uniform_real_distribution<> angle_distrib(0, M_PI);
	double x = x_distrib(rng);
	double y = y_distrib(rng);
	std::shared_ptr<Cell> cell = get_cell_of_position(x,y);
	return std::make_shared<Rod>(x,y, angle_distrib(rng), parameters.rod_width, parameters.rod_length, cell);
}
std::string Medium::to_string() const
{
	std::ostringstream ret;
	ret << std::scientific << parameters.width << " ";
	ret << std::scientific << parameters.height << "\n";
	ret << std::scientific << parameters.rod_width << " ";
	ret << std::scientific << parameters.rod_length << "\n";
	for (const std::shared_ptr<Cell> &c : cells) {
		for (int i = 0; i < c->number_rods_in_cell(); i++) {
			ret << std::scientific << c->get_rod_in_cell(i)->get_x() << " ";
			ret << std::scientific << c->get_rod_in_cell(i)->get_y() << " ";
			ret << std::scientific << c->get_rod_in_cell(i)->get_angle() << "\n";
		}
	}
	return ret.str();
}
std::shared_ptr<Cell> Medium::get_cell_of_rod(const std::shared_ptr<Rod> &rod) const
{
	return get_cell_of_position(rod->get_x(), rod->get_y());
}
std::shared_ptr<Cell> Medium::get_cell_of_position(double x, double y) const
{
	for(std::shared_ptr<Cell> c:cells)
		if(c->check_if_position_in_cell(x,y))
			return c;
	return nullptr;
}
void Medium::initialize_cells()
{
	int N = parameters.number_of_cells_per_direction;
	double width = parameters.width/N;
	double height = parameters.height/N;
	for(int i = 0; i <= N; i++){
		for(int j = 0; j <= N; j++){
			cells.push_back(std::make_shared<Cell>(i*width,j*height,width,height));
		}
	}
	for(const std::shared_ptr<Cell>& c:cells)
	{
		std::vector<std::shared_ptr<Cell>> neighbours = get_neighbours_of_cell(c);
		c->add_neighbours(neighbours);
	}

}
std::vector<std::shared_ptr<Cell>> Medium::get_neighbours_of_cell(const std::shared_ptr<Cell> &cell) const
{
	std::vector<std::shared_ptr<Cell>>ret;
	std::shared_ptr<Cell> upper_neighbour = get_cell_of_position(cell->get_center_x(),cell->get_center_y() + cell->get_height());
	if(upper_neighbour != nullptr)
		ret.push_back(upper_neighbour);
	std::shared_ptr<Cell> lower_neighbour = get_cell_of_position(cell->get_center_x(),cell->get_center_y()-cell->get_height());
	if(lower_neighbour != nullptr)
		ret.push_back(lower_neighbour);
	std::shared_ptr<Cell> left_neighbour = get_cell_of_position(cell->get_center_x()+cell->get_width(),cell->get_center_y());
	if(left_neighbour != nullptr)
		ret.push_back(left_neighbour);
	std::shared_ptr<Cell> right_neighbour = get_cell_of_position(cell->get_center_x()-cell->get_width(),cell->get_center_y());
	if(right_neighbour != nullptr)
		ret.push_back(right_neighbour);
	std::shared_ptr<Cell> topleft_neighbour = get_cell_of_position(cell->get_center_x()-cell->get_width(),cell->get_center_y()+cell->get_height());
	if(topleft_neighbour != nullptr)
		ret.push_back(topleft_neighbour);
	std::shared_ptr<Cell> topright_neighbour = get_cell_of_position(cell->get_center_x()+cell->get_width(),cell->get_center_y()+cell->get_height());
	if(topright_neighbour != nullptr)
		ret.push_back(topright_neighbour);
	std::shared_ptr<Cell> lowleft_neighbour = get_cell_of_position(cell->get_center_x()-cell->get_width(),cell->get_center_y()-cell->get_height());
	if(lowleft_neighbour != nullptr)
		ret.push_back(lowleft_neighbour);
	std::shared_ptr<Cell> lowright_neighbour = get_cell_of_position(cell->get_center_x()+cell->get_width(),cell->get_center_y()-cell->get_height());
	if(lowright_neighbour != nullptr)
		ret.push_back(lowright_neighbour);
	return ret;
}
double Medium::calculate_energy_for_rod(const std::shared_ptr<Rod> &rod) const
{
	double ret = 0;
	double rod_mass_difference = parameters.rod_length * parameters.rod_width * parameters.rod_width *
		(parameters.rod_density - parameters.density);
	std::shared_ptr<Cell> cell_of_rod = rod->get_cell();
	ret += rod->get_y() * parameters.gravity*rod_mass_difference;
	if (rod->get_y() - std::abs(std::sin(rod->get_angle()) * parameters.rod_length) < 0)
		return 1e100;
	if (rod->get_x() - std::abs(std::cos(rod->get_angle()) * parameters.rod_length) < 0)
		return 1e100;
	if (rod->get_x() + std::abs(std::cos(rod->get_angle()) * parameters.rod_length) > parameters.width)
		return 1e100;
	if (rod->get_y() > parameters.height)
		return 1e100;
	for (int j = 0; j < cell_of_rod->number_rods_in_patch(); ++j) {
		if (rod == cell_of_rod->get_rod_in_patch(j))
			continue;
		if (rod->check_collision(cell_of_rod->get_rod_in_patch(j))) {
			return 1e100;
		}
	}
	return ret;
}

double Medium::Movement::calculate_energy_after_movement()
{
	execute_movement();
	double ret = m->calculate_energy();
	reverse_movement();
	return ret;
}

Medium::Movement::Movement(const std::shared_ptr<Medium>  &m, const double time_step, std::mt19937 &rng):m(m)
{

	std::uniform_int_distribution<> cell_distrib(0, m->cells.size()-1);
	changed_cell_index = cell_distrib(rng);
	if(m->cells[changed_cell_index]->number_rods_in_cell() == 0){
		nothing_to_move = true;
		return;
	}
	std::uniform_int_distribution<> rod_distrib(0, m->cells[changed_cell_index]->number_rods_in_cell()-1);
	changed_rod_index_in_cell = rod_distrib(rng);
	rod_changes_cell = false;
	new_cell_index = changed_cell_index;
	changed_cell = m->cells[changed_cell_index];
	changed_rod = changed_cell->get_rod_in_cell(changed_rod_index_in_cell);
	new_cell = changed_cell;

	double amplitude_parallel = std::sqrt(2*m->parameters.diffusion_coefficient_parallel*time_step);
	double amplitude_perpendicular = std::sqrt(2*m->parameters.diffusion_coefficient_perpendicular*time_step);
	double amplitude_rotation = std::sqrt(2*m->parameters.diffusion_coefficient_rotation*time_step);
	std::uniform_real_distribution<> parallel_distrib(-amplitude_parallel, amplitude_parallel);
	std::uniform_real_distribution<> perpendicular_distrib(-amplitude_perpendicular, amplitude_perpendicular);
	std::uniform_real_distribution<> rotation_distrib(-amplitude_rotation, amplitude_rotation);
	parallel_movement = parallel_distrib(rng);
	perpendicular_movement = perpendicular_distrib(rng);
	rotation_movement = rotation_distrib(rng);
}
void Medium::Movement::execute_movement()
{
	if(nothing_to_move)
		return;
	if(!moved) {
		rod_changes_cell = changed_rod->move_rod(parallel_movement, perpendicular_movement, rotation_movement);
		if (rod_changes_cell)
			new_cell = changed_cell->move_rod_to_neighbour(changed_rod);
	}
	moved = true;
}
void Medium::Movement::reverse_movement()
{
	if(nothing_to_move)
		return;
	if(moved) {
		changed_rod->reverse_move_rod(parallel_movement, perpendicular_movement, rotation_movement);
		changed_rod->move_to_cell(changed_cell);
	}
	moved = false;
}
double Medium::Movement::calculate_energy_after_movement_for_rod()
{
	if(nothing_to_move)
		return 0;
	bool moved_before_manipulation = moved;
		execute_movement();
	double ret = m->calculate_energy_for_rod(changed_rod);
		reverse_movement();
	return ret;
}
double Medium::Movement::calculate_energy_before_movement_for_rod()
{
	if(nothing_to_move)
		return 0;
	double ret = m->calculate_energy_for_rod(changed_rod);
	return ret;
}
