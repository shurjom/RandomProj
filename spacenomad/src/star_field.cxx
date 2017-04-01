/*
 * starfield.cxx
 *
 *  Created on: Mar 21, 2015
 *      Author: Kristofer
 */

#include "main.hxx"
#include "star_field.hxx"
#include <algorithm>
#include <iostream>

star_field::star_field(SDL_Renderer *ren)
: field(4096)
, layers {
	star_layer(star_texture_ptrs, SPACE_NOMAD_ARRAY_SIZE(star_texture_ptrs), get_block_side_length(), 0.01),
	star_layer(star_texture_ptrs, SPACE_NOMAD_ARRAY_SIZE(star_texture_ptrs), get_block_side_length(), 0.1),
	star_layer(star_texture_ptrs, SPACE_NOMAD_ARRAY_SIZE(star_texture_ptrs), get_block_side_length(), 0.25),
}
, star_0_texture(loadTexture(ren, "star_field/star_0.png"))
, star_1_texture(loadTexture(ren, "star_field/star_1.png"))
, star_2_texture(loadTexture(ren, "star_field/star_2.png"))
, star_texture_ptrs {
	star_0_texture.get(),
	star_1_texture.get(),
	star_2_texture.get(),
}
{
}

void
star_field::iterate(std::function<void(field_layer&)> visit) {
	for (unsigned i = 0; i < SPACE_NOMAD_ARRAY_SIZE(layers); i++) {
		visit(layers[i]);
	}
}

star_field::~star_field() {
}

star_layer::star_layer(
	SDL_Texture **textures,
	int textures_count,
	int block_side_length,
	double parallax_factor)
: field_layer(
	block_side_length,
	parallax_factor)
, textures_count(textures_count)
, textures(textures)
{
}

void star_layer::add_block(std::default_random_engine& random_engine, int grid_x, int grid_y, SDL_Renderer *ren) {
	std::cerr << "add(" << grid_x << "," << grid_y << ")\n";
	star_blocks.push_back(
			star_block(
					textures,
					textures_count,
					get_block_side_length(),
					random_engine,
					grid_x,
					grid_y));
}

void
star_layer::citerate(std::function<bool(const block&)> visit)
{
	for (int i = star_blocks.size(); i > 0; i--)
		if (visit(star_blocks[i - 1]))
			star_blocks.erase(star_blocks.begin() + i - 1);
}

