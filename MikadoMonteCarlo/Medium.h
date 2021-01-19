//
// Created by tovermodus on 12/2/20.
//

#ifndef _MEDIUM_H_
#define _MEDIUM_H_

#include <vector>
#include <memory>
#include <random>
#include "Rod.h"
#include "Cell.h"
#include "MediumParameters.h"

class Medium {
    private:
	std::vector<std::shared_ptr<Cell>> cells;
	std::shared_ptr<Rod> create_random_rod(std::mt19937 &rng) const;
	bool rod_is_acceptable(const std::shared_ptr<Rod> &rod) const;
	std::shared_ptr<Cell> get_cell_of_rod(const std::shared_ptr<Rod> &rod) const;
std::shared_ptr<Cell> get_cell_of_position(double x, double y) const;
	std::vector<std::shared_ptr<Cell>> get_neighbours_of_cell(const std::shared_ptr<Cell> &cell) const;
	void initialize_rods(std::mt19937 &rng);
	void initialize_cells();

    public:
	const MediumParameters parameters;
	explicit Medium(const MediumParameters &params, std::mt19937 &rng);
	double calculate_energy() const;
	double calculate_energy_for_rod(const std::shared_ptr<Rod> &rod) const;
	std::string to_string() const;

	class Movement {
	    private:
		std::shared_ptr<Medium> m;
		std::size_t changed_cell_index;
		std::size_t changed_rod_index_in_cell;
		bool rod_changes_cell;
		std::size_t new_cell_index;
std::shared_ptr<Rod> changed_rod;
		std::shared_ptr<Cell> changed_cell;
		std::shared_ptr<Cell> new_cell;

		double parallel_movement;
		double perpendicular_movement;
		double rotation_movement;

		bool nothing_to_move = false;

		bool moved = false;

	    public:
		Movement(const std::shared_ptr<Medium> &m, double time_step, std::mt19937 &rng);
		void execute_movement();
		void reverse_movement();
		double calculate_energy_after_movement();
		double calculate_energy_after_movement_for_rod();
		double calculate_energy_before_movement_for_rod();
		void apply_periodic_boundary_conditions_x();
	};

	std::shared_ptr<Cell> get_cell_in_direction(const std::shared_ptr<Cell> &cell, int x_direction,
						    int y_direction) const;
};

#endif //_MEDIUM_H_
