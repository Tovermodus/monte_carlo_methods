//
// Created by tovermodus on 12/10/20.
//

#ifndef MIKADOMONTECARLO__CELL_H_
#define MIKADOMONTECARLO__CELL_H_

#include <vector>
#include <memory>
#include <cassert>
#include <set>
#include "Rod.h"

class Rod; // to avoid circular reference

//cells are so large that there is no possibility that a rod from a cell can move and collide with a rod from a non-neighbouring cell
// i.e. cell_size > 2*rod_size + maximal_movement (for hard potential)
class Cell : public std::enable_shared_from_this<Cell>{
    private:
	std::vector<std::shared_ptr<Rod>> rods_in_cell;
	std::vector<std::shared_ptr<Cell>> neighbour_cells;
	const double width;
	const double height;
	const double x_center;
	const double y_center;

    public:
	Cell(double x, double y, double width, double height);
	bool check_if_rod_in_cell(const std::shared_ptr<Rod>& rod) const;
	std::size_t number_rods_in_cell() const;
	std::size_t number_rods_in_patch() const;
	bool add_rod(const std::shared_ptr<Rod>& rod);
	bool remove_rod(const std::shared_ptr<Rod>& rod);
	std::shared_ptr<Rod> get_rod_in_cell(std::size_t index);
	std::shared_ptr<Rod> get_rod_in_patch(std::size_t index);
	std::shared_ptr<Cell> move_rod_to_neighbour(const std::shared_ptr<Rod> &rod);
	bool rod_in_cell_equals_rod_in_patch(std::size_t index_in_cell, std::size_t index_in_patch) const;
	bool patch_index_is_in_cell(std::size_t patch_index) const;
	std::size_t convert_patch_index_to_cell_index(std::size_t patch_index) const;
	bool check_if_position_in_cell(double x, double y) const;
	double get_width() const;
	double get_height() const;
	double get_center_x() const;
	double get_center_y() const;
	void add_neighbours(const std::vector<std::shared_ptr<Cell>> & neighbours);
};

#endif //MIKADOMONTECARLO__CELL_H_
