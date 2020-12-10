//
// Created by tovermodus on 12/2/20.
//

#ifndef _ROD_H_
#define _ROD_H_

#include <cmath>
#include <array>

class Rod {
private:
	double x;
	double y;
	double phi; //angle between length and x-axis
	double width;
	double length;
public:
	Rod(double initial_x, double initial_y, double initial_phi, double width, double length);
	Rod(const Rod & r);
	void move_x(double delta_x)
	{
		x += delta_x;
	}
	void move_y(double delta_y)
	{
		y += delta_y;
	}
	void move_phi(double delta_phi)
	{
		phi += delta_phi;
		if(phi > M_PI)
			phi -= M_PI;
		if(phi < 0)
			phi += M_PI; //PI should be sufficient, since it makes no difference wether a rod is upside down or not
	}
	bool check_collision(const Rod& other) const;
	std::array<double,4> get_line_information(int line_index) const; //centre of line in x,y, angle, length
	Rod generate_moved_rod(double parallel_movement, double perpendicular_movement, double rotation_movement) const ;

	double get_x() const
	{
		return x;
	}
	double  get_y() const
	{
		return y;
	}
	double get_angle() const
	{
		return phi;
	}
};

#endif //_ROD_H_
