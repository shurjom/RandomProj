/*
 * planet.hxx
 *
 *  Created on: Mar 29, 2015
 *      Author: Kristofer
 */

#ifndef SRC_PLANET_HXX_
#define SRC_PLANET_HXX_

#include "main.hxx"
#include "object.hxx"

class planet : public object {
public:
	planet(
			SDL_Renderer *ren,
			double x,
			double y,
			double xVel,
			double yVel,
			double angularVel,
			double density);
	planet(
			SDL_Renderer *ren,
			space_nomad_SDL_Surface_unique_ptr&& surface,
			double x,
			double y,
			double xVel,
			double yVel,
			double angularVel,
			double density);
	planet(
			planet&& src);
	virtual ~planet();
};

#endif /* SRC_PLANET_HXX_ */
