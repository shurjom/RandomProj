/*
 * field.hxx
 *
 *  Created on: May 25, 2015
 *      Author: Kristofer
 */

#ifndef SRC_FIELD_HXX_
#define SRC_FIELD_HXX_

#include "block.hxx"
#include "camera.hxx"

#include <functional>
#include <random>
#include <vector>

class field_layer
{
public:
	virtual void add_block(std::default_random_engine& random_engine, int grid_x, int grid_y, SDL_Renderer *ren) = 0;
	/**
	 * \param visit
	 *   return true to indicate that the current node should be deleted.
	 */
	virtual void citerate(std::function<bool(const block&)> visit) = 0;
	void draw(SDL_Renderer *ren, const camera& display_camera, std::default_random_engine& random_engine);
	virtual ~field_layer();
protected:
	field_layer(int block_side_length, double parallax_factor = 1.0);
	int get_block_side_length() const;
private:
	int grid_x_min, grid_y_min;
	int grid_x_max, grid_y_max;
	int block_side_length;
	double parallax_factor;
};

/**
 * \brief
 *   A thing that causes blocks to be generated as necessary.
 *
 * Actually, the only useful thing this does is initialize
 * a random_engine for you to pass to a field_layer.
 */
class field
{
public:
	virtual void draw(SDL_Renderer *ren, const camera& display_camera);
	virtual ~field();
protected:
	field(int block_side_length);
	int get_block_side_length() const;
	virtual void iterate(std::function<void(field_layer&)> visit) = 0;
	std::default_random_engine random_engine;
private:
	int block_side_length;
};

#endif /* SRC_FIELD_HXX_ */
