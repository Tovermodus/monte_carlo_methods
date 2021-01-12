//
// Created by tovermodus on 12/10/20.
//

#include "Cell.h"
#include <cassert>
#include <sstream>
std::size_t Cell::number_rods_in_patch() const
{
	std::size_t number = number_rods_in_cell();
	for(const std::shared_ptr<Cell>& c:neighbour_cells)
		number += c->number_rods_in_cell();
	return number;
}
std::shared_ptr<Rod> Cell::get_rod_in_patch(std::size_t index)
{
	if(index < number_rods_in_cell())
		return get_rod_in_cell(index);
	index -= number_rods_in_cell();
	for(const std::shared_ptr<Cell>& c:neighbour_cells)
	{
		if(index < c->number_rods_in_cell())
			return c->get_rod_in_cell(index);
		index -= c->number_rods_in_cell();

	}
	return std::shared_ptr<Rod>();
}
inline std::shared_ptr<Rod> Cell::get_rod_in_cell(std::size_t index)
{
	assert(index < number_rods_in_cell());
	return rods_in_cell[index];
}
std::size_t Cell::number_rods_in_cell() const
{
	return rods_in_cell.size();
}
bool Cell::check_if_position_in_cell(double x, double y) const
{
	return (std::abs(x- x_center) < width/2) && (std::abs(y - y_center) < height/2);
}
Cell::Cell(double x, double y, double width, double height) :x_center(x),y_center(y),width(width),height(height)
{
}
bool Cell::add_rod(const std::shared_ptr<Rod>& rod)
{
	for(const std::shared_ptr<Rod>& r:rods_in_cell)
	{
		if(r == rod)
			return false;
	}
	rods_in_cell.push_back(rod);
	rod->move_to_cell(shared_from_this());
	return true;
}
bool Cell::remove_rod(const std::shared_ptr<Rod>& rod)
{
	for (int i = 0; i < number_rods_in_cell(); i++) {
		if (rods_in_cell[i] == rod) {
			rods_in_cell.erase(rods_in_cell.begin() + i);
			return true;
		}
	}
	return false;
}
std::string Cell::to_string()
{
	std::ostringstream ret;
	ret << "[";
	ret << std::scientific << get_center_x() - width/2;
	ret << ",";
	ret << std::scientific << get_center_x() + width/2;
	ret << "]x[";
	ret << std::scientific << get_center_y() - height/2;
	ret << ",";
	ret << std::scientific << get_center_y() + height/2;
	ret << "]";
	return ret.str();
}
std::shared_ptr<Cell> Cell::move_rod_to_neighbour(const std::shared_ptr<Rod> &rod)
{
	for(const std::shared_ptr<Cell>& neighbour:neighbour_cells)
	{
		if(neighbour->check_if_rod_in_cell(rod))
		{
			rod->move_to_cell(neighbour);
			return neighbour;
		}
	}

	std::cout <<"rod: "<< rod->get_x()<<" " << rod->get_y()<< "    cell: "<<to_string() <<"\n";
	for(const std::shared_ptr<Cell>& neighbour:neighbour_cells)
		std::cout << neighbour->to_string() <<"\n";
	std::cout <<"\n\n";
	throw std::range_error("not moved to a neighbour cell, maybe move too large?");
}
bool Cell::rod_in_cell_equals_rod_in_patch(std::size_t cell_index, std::size_t patch_index) const
{
	if(patch_index_is_in_cell(patch_index))
		return convert_patch_index_to_cell_index(patch_index) == cell_index;
	else
		return false;
}
std::size_t Cell::convert_patch_index_to_cell_index(std::size_t patch_index) const
{
	if(!patch_index_is_in_cell(patch_index))
		throw std::range_error("patch index is not in cell");
	else
		return patch_index;
}
bool Cell::patch_index_is_in_cell(std::size_t patch_index) const
{
	return patch_index < number_rods_in_cell();
}
bool Cell::check_if_rod_in_cell(const std::shared_ptr<Rod> &rod) const
{
	return (std::abs(rod->get_x() - x_center) < width/2) && (std::abs(rod->get_y() - y_center) < height/2);
}
double Cell::get_width() const
{
	return width;
}
double Cell::get_height() const
{
	return height;
}
double Cell::get_center_x() const
{
	return x_center;
}
double Cell::get_center_y() const
{
	return y_center;
}
void Cell::add_neighbours(const std::vector<std::shared_ptr<Cell>> &neighbours)
{
	for(const std::shared_ptr<Cell>& neigh:neighbours)
		neighbour_cells.push_back(neigh);
}
