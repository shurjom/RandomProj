/*
 * field.cxx
 *
 *  Created on: May 25, 2015
 *      Author: Kristofer
 */


#include "field.hxx"
#include <ctime>
#include <functional>

field_layer::field_layer(int block_side_length, double parallax_factor)
: grid_x_min(0)
, grid_y_min(0)
, grid_x_max(0)
, grid_y_max(0)
, block_side_length(block_side_length)
, parallax_factor(parallax_factor)
{
}

int
field_layer::get_block_side_length() const
{
	return block_side_length;
}

void
field_layer::draw(SDL_Renderer *ren, const camera& display_camera, std::default_random_engine& random_engine)
{
	// What two coordinates do we need to hit?
	SDL_Rect visible_area;
	auto parallax_display_camera(display_camera.calculateParallax(parallax_factor));
	parallax_display_camera.get_visible_area(&visible_area);
	int new_grid_x_min, new_grid_y_min;
	block::snap(block_side_length, visible_area.x, visible_area.y, new_grid_x_min, new_grid_y_min);
	int new_grid_x_max, new_grid_y_max;
	block::snap(block_side_length, visible_area.x + visible_area.w, visible_area.y + visible_area.h, new_grid_x_max, new_grid_y_max);

	// Widen block area by 1 so that stars just outside the screen do not change randomly.
	new_grid_x_min--;
	new_grid_y_min--;
	new_grid_x_max++;
	new_grid_y_max++;
	//std::cerr << "min-=(" << new_grid_x_min << "," << new_grid_y_min << "), max+=(" << new_grid_x_max << "," << new_grid_y_max << ")\n";

	bool no_blocks = true;
	bool *no_blocks_ref = &no_blocks;

	// Drop blocks outside our displayed/kept area.
	citerate(
			[no_blocks_ref, new_grid_x_max, new_grid_x_min, new_grid_y_max, new_grid_y_min] (const block& a_block) -> bool {
		//std::cerr << "have (" << star_block_i->grid_x << "," << star_block_i->grid_y << ")\n";
		*no_blocks_ref = false;
		return a_block.get_grid_x() > new_grid_x_max
				|| a_block.get_grid_x() < new_grid_x_min
				|| a_block.get_grid_y() > new_grid_y_max
				|| a_block.get_grid_y() < new_grid_y_min;
	});

	// Intersect our rectangles if they overlap. Otherwise, initialize to a point.
	if (no_blocks)
		// No overlap, reset.
	{
		//std::cerr << "reset\n";
		grid_x_min = grid_x_max = new_grid_x_min;
		grid_y_min = grid_y_max = new_grid_y_min;
		add_block(random_engine, grid_x_min, grid_y_min, ren);
	}
	else
		// Overlap! Intersect
	{
		grid_x_min = std::max(grid_x_min, new_grid_x_min);
		grid_y_min = std::max(grid_y_min, new_grid_y_min);
		grid_x_max = std::min(grid_x_max, new_grid_x_max);
		grid_y_max = std::min(grid_y_max, new_grid_y_max);
	}
	//std::cerr << "grid_min=(" << grid_x_min << "," << grid_y_min << "), grid_max=(" << grid_x_max << "," << grid_y_max <<")\n";

	// Grow new blocks as necessary.
	for (; grid_x_min > new_grid_x_min; grid_x_min--)
		for (auto grid_y = grid_y_min; grid_y <= grid_y_max; grid_y++)
			add_block(random_engine, grid_x_min - 1, grid_y, ren);
	for (; grid_y_min > new_grid_y_min; grid_y_min--)
		for (auto grid_x = grid_x_min; grid_x <= grid_x_max; grid_x++)
			add_block(random_engine, grid_x, grid_y_min - 1, ren);

	for (; grid_x_max < new_grid_x_max; grid_x_max++)
		for (auto grid_y = grid_y_min; grid_y <= grid_y_max; grid_y++)
			add_block(random_engine, grid_x_max + 1, grid_y, ren);
	for (; grid_y_max < new_grid_y_max; grid_y_max++)
		for (auto grid_x = grid_x_min; grid_x <= grid_x_max; grid_x++)
			add_block(random_engine, grid_x, grid_y_max + 1, ren);

	//std::err << "num_blocks=" << star_blocks.size() << "\n";

	// After ensuring that there are things to draw, actually
	// draw those things.
	citerate([&ren, &parallax_display_camera, &random_engine](const block& block) -> bool {
		block.draw(ren, parallax_display_camera);
		return false;
	});
}

field_layer::~field_layer() {
}

field::field(int block_side_length)
: random_engine(time(NULL))
, block_side_length(block_side_length)
{
}

int
field::get_block_side_length() const {
	return block_side_length;
}


void field::draw(SDL_Renderer* ren, const camera& display_camera) {
	iterate(
		[this, ren, display_camera] (field_layer& the_field_layer) {
		the_field_layer.draw(ren, display_camera, random_engine);
	});
}


field::~field() {
}
