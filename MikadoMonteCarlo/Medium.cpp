//
// Created by tovermodus on 12/2/20.
//

#include <sstream>
#include "Medium.h"
Medium::Medium(const MediumParameters &params, std::mt19937 &rng) : parameters(params)

{
	initialize_cells();
	initialize_rods(rng);
	if (cells[0]->get_height() < params.rod_length || cells[0]->get_height() < params.rod_length)
		throw std::domain_error("rods are larger than cells");
}
void Medium::initialize_rods(std::mt19937 &rng)
{
	int iterations = 0;
	int total_number_of_rods = (int)(parameters.rods_per_volume * parameters.width * parameters.height);
	int rod_number = 0;
	while (rod_number < total_number_of_rods && iterations < parameters.rod_placing_max_iterations) {
		std::shared_ptr<Rod> new_rod = create_random_rod(rng);
		std::shared_ptr<Cell> cell = get_cell_of_rod(new_rod);
		cell->add_rod(new_rod);
		if (!rod_is_acceptable(new_rod)) {
			cell->remove_rod(new_rod);
		} else {
			rod_number++;
		}
		iterations++;
	}
	std::cout << rod_number << "rods placed\n";
}
double Medium::calculate_energy_for_rod(const std::shared_ptr<Rod> &rod) const
{
	double ret = 0;
	double rod_mass_difference = parameters.rod_length * parameters.rod_width * parameters.rod_width *
				     (parameters.rod_density - parameters.density);
	ret += rod->get_y() * parameters.gravity * rod_mass_difference;
	if (parameters.ellipsoidal_potential != 0)
		ret += calculate_ellipsoidal_potential(rod);
	if (!rod_is_acceptable(rod))
		return 1e200;
	return ret;
}
double Medium::calculate_energy() const
{
	double ret = 0;
	for (const std::shared_ptr<Cell> &cell : cells) {
		for (int i = 0; i < cell->number_rods_in_cell(); ++i) {
			std::shared_ptr<Rod> r = cell->get_rod_in_cell(i);
			ret += calculate_energy_for_rod(r);
		}
	}
	return ret;
}
bool Medium::rod_is_acceptable(const std::shared_ptr<Rod> &rod, bool log_reason) const
{
	std::shared_ptr<Cell> cell_of_rod = rod->get_cell();
	if (rod->get_y() - std::abs(std::sin(rod->get_angle()) * parameters.rod_length) < 0) {
		if (log_reason)
			std::cout << "y-overflow bottom ";
		return false;
	}
	if (rod->get_y() + std::abs(std::sin(rod->get_angle()) * parameters.rod_length) > parameters.height) {
		if (log_reason)
			std::cout << "y-overflow top ";
		return false;
	}
	if (!parameters.periodic_boundary_conditions) {
		if (rod->get_x() - std::abs(std::cos(rod->get_angle()) * parameters.rod_length) < 0) {
			if (log_reason)
				std::cout << "x-overflow left ";
			return false;
		}
		if (rod->get_x() + std::abs(std::cos(rod->get_angle()) * parameters.rod_length) > parameters.width) {
			if (log_reason)
				std::cout << "x-overflow right ";
			return false;
		}
	}
	for (int j = 0; j < cell_of_rod->number_rods_in_patch(); ++j) {
		if (rod == cell_of_rod->get_rod_in_patch(j))
			continue;
		if (rod->check_collision(cell_of_rod->get_rod_in_patch(j))) {
			if (log_reason)
				std::cout << "collision ";
			return false;
		}
	}
	return true;
}
std::shared_ptr<Rod> Medium::create_random_rod(std::mt19937 &rng) const
{
	std::uniform_real_distribution<> x_distrib(0, parameters.width);
	std::uniform_real_distribution<> y_distrib(0, parameters.height);
	std::uniform_real_distribution<> angle_distrib(0, M_PI);
	double x = x_distrib(rng);
	double y = y_distrib(rng);
	std::shared_ptr<Cell> cell = get_cell_of_position(x, y);
	return std::make_shared<Rod>(x, y, angle_distrib(rng), parameters.rod_width, parameters.rod_length, cell);
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
	for (std::shared_ptr<Cell> c : cells)
		if (c->check_if_position_in_cell(x, y))
			return c;
	return nullptr;
}
void Medium::initialize_cells()
{
	int N = parameters.number_of_cells_per_direction;
	double width = parameters.width / N;
	double height = parameters.height / N;
	if (parameters.periodic_boundary_conditions) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cells.push_back(std::make_shared<Cell>(i * width + width / 2, j * height + height / 2,
								       width, height));
			}
		}
	} else {
		for (int i = 0; i <= N; i++) {
			for (int j = 0; j <= N; j++) {
				cells.push_back(std::make_shared<Cell>(i * width, j * height, width, height));
			}
		}
	}
	for (const std::shared_ptr<Cell> &c : cells) {
		std::vector<std::shared_ptr<Cell>> neighbours = get_neighbours_of_cell(c);
		c->add_neighbours(neighbours);
	}
}
std::shared_ptr<Cell> Medium::get_cell_in_direction(const std::shared_ptr<Cell> &cell, int x_direction,
						    int y_direction) const
{
	double cell_x_pos = cell->get_center_x();
	double cell_y_pos = cell->get_center_y();
	if (x_direction > 0)
		cell_x_pos += cell->get_width();
	if (x_direction < 0)
		cell_x_pos -= cell->get_width();
	if (y_direction > 0)
		cell_y_pos += cell->get_height();
	if (y_direction < 0)
		cell_y_pos -= cell->get_height();

	while (cell_x_pos < 0)
		cell_x_pos += parameters.width;
	while (cell_x_pos > parameters.width)
		cell_x_pos -= parameters.width;
	while (cell_y_pos < 0)
		cell_y_pos += parameters.height;
	while (cell_y_pos > parameters.height)
		cell_y_pos -= parameters.height;
	return get_cell_of_position(cell_x_pos, cell_y_pos);
}
std::vector<std::shared_ptr<Cell>> Medium::get_neighbours_of_cell(const std::shared_ptr<Cell> &cell) const
{
	std::vector<std::shared_ptr<Cell>> ret;
	ret.push_back(get_cell_in_direction(cell, 1, 0));
	ret.push_back(get_cell_in_direction(cell, -1, 0));
	ret.push_back(get_cell_in_direction(cell, 0, 1));
	ret.push_back(get_cell_in_direction(cell, 0, -1));
	ret.push_back(get_cell_in_direction(cell, 1, 1));
	ret.push_back(get_cell_in_direction(cell, 1, -1));
	ret.push_back(get_cell_in_direction(cell, -1, 1));
	ret.push_back(get_cell_in_direction(cell, -1, -1));
	for (const auto &c : ret)
		if (c == nullptr || ret.size() != 8) {
			std::cout << "this: " << cell->to_string() << "\n";
			for (const auto &n : ret)
				if (n != nullptr)
					std::cout << "neighbours: " << n->to_string() << "\n";
				else
					std::cout << "neighbours: nullptr\n";
			throw std::domain_error("something witht the domain went wrong");
		}
	return ret;
}
double Medium::calculate_ellipsoidal_potential(const std::shared_ptr<Rod> &rod) const
{
	double ret = 0;
	std::shared_ptr<Cell> cell_of_rod = rod->get_cell();
	for (int j = 0; j < cell_of_rod->number_rods_in_patch(); ++j) {
		std::shared_ptr<Rod> other = cell_of_rod->get_rod_in_patch(j);
		if (other->get_x() - rod->get_x() > 2 * parameters.rod_length ||
		    other->get_y() - rod->get_y() > 2 * parameters.rod_length)
			continue;
		if (rod->ellipsoid_radius(other->get_x(), other->get_y()) == 0)
			continue;
		ret += parameters.ellipsoidal_potential /
		       std::pow(rod->ellipsoid_radius(other->get_x(), other->get_y()), 6);
		ret += parameters.ellipsoidal_potential /
		       std::pow(other->ellipsoid_radius(rod->get_x(), rod->get_y()), 6);
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

Medium::Movement::Movement(const std::shared_ptr<Medium> &m, const double time_step, std::mt19937 &rng) : m(m)
{
	std::uniform_int_distribution<> cell_distrib(0, m->cells.size() - 1);
	changed_cell_index = cell_distrib(rng);
	if (m->cells[changed_cell_index]->number_rods_in_cell() == 0) {
		nothing_to_move = true;
		return;
	}
	std::uniform_int_distribution<> rod_distrib(0, m->cells[changed_cell_index]->number_rods_in_cell() - 1);
	changed_rod_index_in_cell = rod_distrib(rng);
	rod_changes_cell = false;
	new_cell_index = changed_cell_index;
	changed_cell = m->cells[changed_cell_index];
	changed_rod = changed_cell->get_rod_in_cell(changed_rod_index_in_cell);
	new_cell = changed_cell;

	double amplitude_parallel = std::sqrt(2 * m->parameters.diffusion_coefficient_parallel * time_step);
	double amplitude_perpendicular = std::sqrt(2 * m->parameters.diffusion_coefficient_perpendicular * time_step);
	double amplitude_rotation = std::sqrt(2 * m->parameters.diffusion_coefficient_rotation * time_step);
	std::uniform_real_distribution<> parallel_distrib(-amplitude_parallel, amplitude_parallel);
	std::uniform_real_distribution<> perpendicular_distrib(-amplitude_perpendicular, amplitude_perpendicular);
	std::uniform_real_distribution<> rotation_distrib(-amplitude_rotation, amplitude_rotation);
	parallel_movement = parallel_distrib(rng);
	perpendicular_movement = perpendicular_distrib(rng);
	rotation_movement = rotation_distrib(rng);
}
void Medium::Movement::execute_movement()
{
	if (nothing_to_move)
		return;
	if (!moved) {
		rod_changes_cell = changed_rod->move_rod(parallel_movement, perpendicular_movement, rotation_movement);
		boundary_movement =
			changed_rod->apply_periodic_boundary_conditions(m->parameters.width, m->parameters.height);
		if (rod_changes_cell)
			new_cell = changed_cell->move_rod_to_neighbour(changed_rod);
	}
	moved = true;
}
void Medium::Movement::reverse_movement()
{
	if (nothing_to_move)
		return;
	if (moved) {
		changed_rod->reverse_move_rod(parallel_movement, perpendicular_movement, rotation_movement);
		changed_rod->reverse_boundary_movement(boundary_movement);
		changed_rod->move_to_cell(changed_cell);
	}
	moved = false;
}
double Medium::Movement::calculate_energy_after_movement_for_rod()
{
	if (nothing_to_move)
		return 0;
	bool moved_before_manipulation = moved;
	execute_movement();
	double ret = m->calculate_energy_for_rod(changed_rod);
	reverse_movement();
	return ret;
}
double Medium::Movement::calculate_energy_before_movement_for_rod()
{
	if (nothing_to_move)
		return 0;
	double ret = m->calculate_energy_for_rod(changed_rod);
	return ret;
}
