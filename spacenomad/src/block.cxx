/*
 * head.cxx
 *
 *  Created on: May 25, 2015
 *      Author: Kristofer
 */

#include "block.hxx"

block::block(int block_side_length, int grid_x, int grid_y)
: block_side_length(block_side_length)
, grid_x(grid_x)
, grid_y(grid_y)
{
}


int block::get_block_side_length() const {
	return block_side_length;
}

double block::get_x() const {
	return block_side_length * (double)grid_x;
}

double block::get_y() const {
	return block_side_length * (double)grid_y;
}

int block::get_grid_x() const {
	return grid_x;
}

int block::get_grid_y() const {
	return grid_y;
}

block::~block() {
}

void block::snap(int block_size_length, double x, double y, int& grid_x, int& grid_y) {
	grid_x = (int)x/block_size_length;
	grid_y = (int)y/block_size_length;
}
