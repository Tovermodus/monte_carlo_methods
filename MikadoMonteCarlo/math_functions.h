//
// Created by tovermodus on 12/4/20.
//

#ifndef MIKADOMONTECARLO__MATH_FUNCTIONS_H_
#define MIKADOMONTECARLO__MATH_FUNCTIONS_H_

#include <cmath>

bool lines_intersect_in_range(double angle1, double angle2,       //angle to x-axis
			      double central_x1, double central_x2, //central point of lines in x-direction
			      double central_y1, double central_y2, //central point of lines in y-direction
			      double radius1, double radius2) //length in which intersection must happen
{
	double difference_x = central_x2 - central_x1;
	double difference_y = central_y2 - central_y1;
	if(angle1 == angle2)
		return atan2(difference_y, difference_x) == angle1;
	double distance_from_center2 = (cos(angle1)*difference_y-sin(angle1)*difference_x)
		/(cos(angle2)*sin(angle1) - sin(angle2)*cos(angle1));
	double distance_from_center1 = (-cos(angle2)*difference_y+sin(angle2)*difference_x)
		/(cos(angle1)*sin(angle2) - sin(angle1)*cos(angle2));
	return std::abs(distance_from_center1) <= radius1 && std::abs(distance_from_center2) <= radius2;
}
#endif //MIKADOMONTECARLO__MATH_FUNCTIONS_H_
