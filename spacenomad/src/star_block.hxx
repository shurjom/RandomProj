/*
 * star_block.hxx
 *
 *  Created on: Mar 21, 2015
 *      Author: Kristofer
 */

#ifndef SRC_STAR_BLOCK_HXX_
#define SRC_STAR_BLOCK_HXX_

extern "C" {
#include <SDL.h>
}

#include "block.hxx"
#include "camera.hxx"

#include <random>
#include <vector>

struct star {
	int x;
	int y;
	int texture_i;
};

class star_block
: public block {
public:
	star_block(
		SDL_Texture **textures, int texture_count, int block_size_length, std::default_random_engine& random_engine,
		int grid_x, int grid_y);
    void draw(SDL_Renderer *ren, const camera& display_camera) const;
    virtual ~star_block();
private:
	std::vector<star> stars;
	SDL_Texture **textures;
};

#endif /* SRC_STAR_BLOCK_HXX_ */
