/*
 * planet.cxx
 *
 *  Created on: Mar 29, 2015
 *      Author: Kristofer
 */

#include "planet.hxx"
#include <cmath>

planet::planet(
		SDL_Renderer *ren,
		space_nomad_SDL_Surface_unique_ptr&& surface,
		double x,
		double y,
		double xVel,
		double yVel,
		double angularVel,
		double density)
: object(ren, std::move(surface), x, y, xVel, yVel, angularVel) {
	mass = density * pow(radius, 2) * M_PI;
	gravitational_radius_of_influence = 10000000;
	attractive = true;
}

planet::planet(SDL_Renderer *ren, double x, double y, double xVel, double yVel, double angularVel, double density)
: planet(ren, loadSurface("planet/plain.png"), x, y, xVel, yVel, angularVel, density) {
}

planet::planet(
		planet&& src)
: object(std::move(src)) {
}


planet::~planet() {
}
