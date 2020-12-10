//
// Created by tovermodus on 12/2/20.
//

#include <iostream>
#include "Rod.h"
#include "math_functions.h"
bool Rod::check_collision (const Rod& other) const
{
	std::array<double,4> line_information_this{}, line_information_other{};
	for (int i = 0; i < 4; i++) {
		line_information_this = get_line_information(i);
		for (int j = 0; j < 4; j++) {
			line_information_other = other.get_line_information(j);
			if(lines_intersect_in_range(line_information_this[2],line_information_other[2],
						     line_information_this[0],line_information_other[0],
						     line_information_this[1],line_information_other[1],
						     line_information_this[3]/2,line_information_other[3]/2)) {
				return true;
			}
		}
	}
	return false;
}
Rod::Rod(double initial_x, double initial_y, double initial_phi, double width, double length)
{
	this->x = initial_x;
	this->y = initial_y;
	this->phi = initial_phi;
	this->width = width;
	this->length = length;
}
std::array<double, 4> Rod::get_line_information(int line_index) const
{
	double center_x;
	double center_y;
	double angle;
	double len;
	switch (line_index) {
	case 0:
		center_x = x - width/2*sin(phi);
		center_y = y + width/2*cos(phi);
		angle = phi;
		len = length;
		break;
	case 1:
		center_x = x + width/2*sin(phi);
		center_y = y - width/2*cos(phi);
		angle = phi;
		len = length;
		break;
	case 2:
		center_x = x + length/2*cos(phi);
		center_y = y + length/2*sin(phi);
		angle = phi+M_PI/2;
		len = width;
		break;
	case 3:
		center_x = x - length/2*cos(phi);
		center_y = y - length/2*sin(phi);
		angle = phi+M_PI/2;
		len = width;
		break;
	default:
		throw "Not a valid line";
	}
	if(angle > M_PI)
		angle -= M_PI;
	return std::array<double,4>{center_x,center_y,angle,len};
}
Rod Rod::generate_moved_rod(double parallel_movement, double perpendicular_movement, double rotation_movement) const
{
	Rod moved_rod = Rod(*this);
	moved_rod.move_x(parallel_movement*std::cos(phi));
	moved_rod.move_y(parallel_movement*std::sin(phi));
	moved_rod.move_x(perpendicular_movement*std::cos(phi+M_PI));
	moved_rod.move_y(perpendicular_movement*std::sin(phi+M_PI));
	moved_rod.move_phi(rotation_movement);
	return moved_rod;
}
Rod::Rod(const Rod & r)
{
	this->x = r.x;
	this->y = r.y;
	this->phi = r.phi;
	this->width = r.width;
	this->length = r.length;
}
