/*
 * block.hxx
 *
 *  Created on: May 25, 2015
 *      Author: Kristofer
 */

#ifndef SRC_BLOCK_HXX_
#define SRC_BLOCK_HXX_

#include "camera.hxx"

extern "C" {
#include <SDL.h>
};

class block
{
public:
	block(int block_side_length, int grid_x, int grid_y);
	int get_block_side_length() const;
	double get_x() const;
	double get_y() const;
	int get_grid_x() const;
	int get_grid_y() const;
	virtual void draw(SDL_Renderer *ren, const camera& display_camera) const = 0;
	/**
	 * \brief Yield the grid_x and grid_y values of the block that should be created if x and y need to be displayed on-screen.
	 */
	static void snap(int block_size_length, double x, double y, int& grid_x, int& grid_y);
	virtual ~block();
private:
	int block_side_length;
	int grid_x;
	int grid_y;
};

#endif /* SRC_BLOCK_HXX_ */
