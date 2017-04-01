/*
 * brush_creation.hxx
 *
 *  Created on: Jun 24, 2015
 *      Author: Kristofer
 */

#ifndef SRC_PLANET_GENERATION_BRUSH_HXX_
#define SRC_PLANET_GENERATION_BRUSH_HXX_

#include "camera.hxx"
#include "main.hxx"
#include "mode.hxx"
#include "player.hxx"
#include "star_field.hxx"
#include "universe.hxx"

#include <vector>



namespace spacenomad {

class brush {
public:
	static void fill_circle(SDL_Renderer *ren, const SDL_Rect& bounds, bool inverted = false);
	brush(std::default_random_engine& random_engine);
	brush(brush&& orig);
	space_nomad_SDL_Surface_unique_ptr& get_surface() { return surface; }
	virtual ~brush();
private:
	space_nomad_SDL_Surface_unique_ptr surface;
};

}; /* namespace spacenomad */

#endif /* SRC_PLANET_GENERATION_BRUSH_HXX_ */
