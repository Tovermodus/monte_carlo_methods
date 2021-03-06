//
// Created by tovermodus on 12/2/20.
//

#include <iostream>
#include <utility>
#include "Rod.h"
#include "math_functions.h"
bool Rod::check_collision(const std::shared_ptr<Rod> &other) const
{
	if (std::abs(get_x() - other->get_x()) + std::abs(get_y() - other->get_y()) > length + width)
		return false;
	std::array<double, 4> line_information_this{}, line_information_other{};
	for (int i = 0; i < 4; i++) {
		line_information_this = get_line_information(i);
		for (int j = 0; j < 4; j++) {
			line_information_other = other->get_line_information(j);
			if (lines_intersect_in_range(line_information_this[2], line_information_other[2],
						     line_information_this[0], line_information_other[0],
						     line_information_this[1], line_information_other[1],
						     line_information_this[3] / 2, line_information_other[3] / 2)) {
				return true;
			}
		}
	}
	return false;
}
Rod::Rod(double initial_x, double initial_y, double initial_phi, double width, double length,
	 const std::shared_ptr<Cell> &cell)
{
	this->x = initial_x;
	this->y = initial_y;
	this->phi = initial_phi;
	this->width = width;
	this->length = length;
	this->cell = cell;
}
std::array<double, 4> Rod::get_line_information(int line_index) const
{
	double center_x;
	double center_y;
	double angle;
	double len;
	switch (line_index) {
	case 0:
		center_x = x - width / 2 * sin(phi);
		center_y = y + width / 2 * cos(phi);
		angle = phi;
		len = length;
		break;
	case 1:
		center_x = x + width / 2 * sin(phi);
		center_y = y - width / 2 * cos(phi);
		angle = phi;
		len = length;
		break;
	case 2:
		center_x = x + length / 2 * cos(phi);
		center_y = y + length / 2 * sin(phi);
		angle = phi + M_PI / 2;
		len = width;
		break;
	case 3:
		center_x = x - length / 2 * cos(phi);
		center_y = y - length / 2 * sin(phi);
		angle = phi + M_PI / 2;
		len = width;
		break;
	default:
		throw std::logic_error("not a valid line");
	}
	if (angle > M_PI)
		angle -= M_PI;
	return std::array<double, 4>{ center_x, center_y, angle, len };
}
bool Rod::move_rod(double parallel_movement, double perpendicular_movement, double rotation_movement)
{
	//std::cout << "\n";
	//std::cout << to_string() << "before movement\n";
	move_x(parallel_movement * std::cos(phi));
	move_y(parallel_movement * std::sin(phi));
	move_x(perpendicular_movement * std::cos(phi + M_PI));
	move_y(perpendicular_movement * std::sin(phi + M_PI));
	move_phi(rotation_movement);

	//std::cout << to_string() << "after movement\n";
	return !cell->check_if_rod_in_cell(shared_from_this());
}
Rod::Rod(const Rod &r) : enable_shared_from_this(r)
{
	this->x = r.x;
	this->y = r.y;
	this->phi = r.phi;
	this->width = r.width;
	this->length = r.length;
	this->cell = r.cell;
}
void Rod::move_x(double delta_x)
{
	x += delta_x;
}
void Rod::move_y(double delta_y)
{
	y += delta_y;
}
void Rod::move_phi(double delta_phi)
{
	phi += delta_phi;
	if (phi > M_PI)
		phi -= M_PI;
	if (phi < 0)
		phi += M_PI; //PI should be sufficient, since it makes no difference wether a rod is upside down or not
}
bool Rod::move_to_cell(const std::shared_ptr<Cell> &new_cell)
{
	if (cell == new_cell)
		return false;
	cell->remove_rod(shared_from_this());
	cell = new_cell;
	new_cell->add_rod(shared_from_this());
	return true;
}
std::shared_ptr<Cell> Rod::get_cell() const
{
	return cell;
}
bool Rod::reverse_move_rod(double parallel_movement, double perpendicular_movement, double rotation_movement)
{
	//std::cout << to_string() << "before movement\n";
	move_phi(-rotation_movement); //rotation does not commute
	move_x(-parallel_movement * std::cos(phi)); //translation is commutative
	move_y(-parallel_movement * std::sin(phi));
	move_x(-perpendicular_movement * std::cos(phi + M_PI));
	move_y(-perpendicular_movement * std::sin(phi + M_PI));
	//std::cout << to_string() << "after movement\n";
	return cell->check_if_rod_in_cell(shared_from_this());
}
std::pair<double, double> rod_movement_from_cartesian_movement(double angle, double move_x, double move_y)
{
	double parralel = inner_product(move_x, move_y, std::cos(angle), std::sin(angle));
	double perpendicular = inner_product(move_x, move_y, std::cos(angle + M_PI), std::sin(angle + M_PI));
	return { parralel, perpendicular };
}
std::pair<double, double> Rod::apply_periodic_boundary_conditions(double medium_width, double medium_height)
{
	double lastx = x;
	double lasty = y;
	//std::cout << to_string() << "before boundary\n";
	//std::cout << std::scientific << medium_width << " " << std::scientific << medium_height << "\n";
	while (x < 0)
		x += medium_width;
	while (x > medium_width)
		x -= medium_width;
	while (y < 0)
		y = 1e-10;
	while (y > medium_height)
		y = medium_height - 1e-10;
	//std::cout << to_string() <<"after boundary\n";
	return { x - lastx, y - lasty };
}
std::string Rod::to_string() const
{
	std::ostringstream ret;
	ret << std::scientific << get_x();
	ret << " ";
	ret << std::scientific << get_y();
	ret << " ";
	ret << std::scientific << get_angle();
	return ret.str();
}
void Rod::reverse_boundary_movement(std::pair<double, double> movement)
{
	move_x(-movement.first);
	move_y(-movement.second);
}
double Rod::ellipsoid_radius(double x, double y) const
{
	double transformed_x = inner_product(x - get_x(), y - get_y(), std::cos(get_angle()), std::sin(get_angle()));
	double transformed_y =
		inner_product(x - get_x(), y - get_y(), std::cos(get_angle() + M_PI), std::sin(get_angle() + M_PI));
	return 2 * std::sqrt(std::pow(transformed_x / length, 2) + std::pow(transformed_y / width, 2));
}
