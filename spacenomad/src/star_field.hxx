/*
 * starfield.hxx
 *
 *  Created on: Mar 21, 2015
 *      Author: Kristofer
 */

#ifndef SRC_STAR_FIELD_HXX_
#define SRC_STAR_FIELD_HXX_


extern "C" {
#include <SDL.h>
}

#include "field.hxx"
#include "main.hxx"
#include "star_block.hxx"
#include <vector>


class star_layer
: public field_layer {
public:
	star_layer(SDL_Texture **textures, int textures_count, int block_side_length, double parallax_factor);
	void add_block(std::default_random_engine& random_engine, int grid_x, int grid_y, SDL_Renderer *ren);
	void citerate(std::function<bool(const block&)> visit);
private:
	std::vector<star_block> star_blocks;
	int textures_count;
	SDL_Texture **textures;
};

class star_field
: public field {
public:
	star_field(SDL_Renderer *ren);
	void iterate(std::function<void(field_layer&)> visit);
	virtual ~star_field();
private:
	star_layer layers[3];
	space_nomad_SDL_Texture_unique_ptr star_0_texture;
	space_nomad_SDL_Texture_unique_ptr star_1_texture;
	space_nomad_SDL_Texture_unique_ptr star_2_texture;
	SDL_Texture *star_texture_ptrs[3];
};


#endif /* SRC_STAR_FIELD_HXX_ */
