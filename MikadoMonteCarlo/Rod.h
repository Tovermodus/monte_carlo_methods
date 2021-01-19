//
// Created by tovermodus on 12/2/20.
//

#ifndef _ROD_H_
#define _ROD_H_

#include <cmath>
#include <array>
#include <iostream>
#include <memory>
#include "Cell.h"
#include <sstream>
class Cell;

class Rod : public std::enable_shared_from_this<Rod> {
    private:
	double x;
	double y;
	double phi; //angle between length and x-axis
	double width;
	double length;
	std::shared_ptr<Cell> cell;
	void move_x(double delta_x);
	void move_y(double delta_y);
	void move_phi(double delta_phi);

    public:
	Rod(double initial_x, double initial_y, double initial_phi, double width, double length,
	    const std::shared_ptr<Cell> &cell);
	Rod(const Rod &r);
	std::string to_string() const;
	bool check_collision(const std::shared_ptr<Rod> &other) const;
	std::array<double, 4> get_line_information(int line_index) const; //centre of line in x,y, angle, length
	bool move_to_cell(const std::shared_ptr<Cell> &new_cell);
	double get_x() const
	{
		return x;
	}
	double get_y() const
	{
		return y;
	}
	double get_angle() const
	{
		return phi;
	}
	std::shared_ptr<Cell> get_cell() const;
	bool move_rod(double parallel_movement, double perpendicular_movement, double rotation_movement);
	bool reverse_move_rod(double parallel_movement, double perpendicular_movement, double rotation_movement);
	std::pair<double, double> apply_periodic_boundary_conditions(double medium_width, double medium_height);
	void reverse_boundary_movement(std::pair<double, double> boundary_movement);
};

#endif //_ROD_H_
