/*
 * object.cpp
 *
 *  Created on: Mar 20, 2015
 *      Author: Kristofer
 */

#include "object.hxx"
#include "universe.hxx"

#include <algorithm>
#include <iostream>
#include <math.h>

object::object(
		SDL_Renderer *ren,
		space_nomad_SDL_Surface_unique_ptr&& surface,
		double x,
		double y,
		double xVel,
		double yVel,
		double angularVel,
		double mass)
: x(x)
, y(y)
, xVel(xVel)
, yVel(yVel)
, facingDirection(0)
, angularVel(angularVel)
, mass(mass)
, texture(createTexture(ren, surface))
  // Given two objects that have this object’s mass, what is the
  // maximum distance they can be from each other to at least have an
  // attractive force of 0.01 «units?»?
  //
  // F = {G M_1 M_2}\over{r^2}
  //
  // Solve for r when F=0.01
  //
  // r = \sqrt\(F\over{G M_1 M_2}\)
  // r = \sqrt\(0.01\over{G M_1 M_2}\)
  //
  // Assuming M_1 is M_2:
  //
  // r = \sqrt\(0.01\over{G \{M_1\}^2}\)
  // r = \sqrt\(0.01\over G\)\over M_1
, gravitational_radius_of_influence(sqrt(0.01/universe::G)/mass)
{
        int w;
        SDL_QueryTexture(texture.get(), NULL, NULL, &w, NULL);
	radius = w/2;
	attractive = false;
}

object::object(
		SDL_Renderer *ren,
		const char *textureFilename,
		double x,
		double y,
		double xVel,
		double yVel,
		double angularVel,
		double mass)
: object(
		ren,
		loadSurface(textureFilename),
		x,
		y,
		xVel,
		yVel,
		angularVel,
		mass) {
}

object::object(
		object&& src) {
	x = src.x;
	y = src.y;
	xVel = src.xVel;
	yVel = src.yVel;
	angularVel = src.angularVel;
	mass = src.mass;
	texture = std::move(src.texture);
}

// This is for making a number that is close enough to zero become zero.
static bool min_to_zero(double* number, double min) {
	if (*number < min && *number > -min) {
		number = 0;
		return true;
	}
	return false;
}

void object::angularVel_dampening(double dampening){
	if (!min_to_zero(&angularVel, .001)) {
		angularVel /= dampening;
	}
}

void object::movement_dampening(double dampening) {
	auto const minSpeed = .001;
	min_to_zero(&xVel, minSpeed);
	min_to_zero(&yVel, minSpeed);
}

void object::animate() {
	x += xVel;
	y += yVel;
	facingDirection += angularVel;
	facingDirection = fmod(facingDirection, 360);
}

void object::considerCamera(camera& displayCamera, int clearFactor)
{
	int w, h;
	SDL_QueryTexture(texture.get(), NULL, NULL, &w, &h);
	displayCamera.considerObject(x, y, clearFactor * std::max(w, h));
}

void object::draw(SDL_Renderer *ren, const camera& displayCamera) const {
	SDL_Rect dst;
	SDL_QueryTexture(texture.get(), NULL, NULL, &dst.w, &dst.h);
	dst.x = x - dst.w/2;
	dst.y = y - dst.h/2;
	if (displayCamera.transform(&dst))
		SDL_RenderCopyEx(ren, texture.get(), NULL, &dst, facingDirection, NULL, SDL_FLIP_NONE);
}



void object::applyForce(double magnitude, double x, double y) {

	auto acceleration_magnitude = magnitude / mass / space_nomad_fps;
	xVel += acceleration_magnitude * x;
	yVel += acceleration_magnitude * y;
}

object::~object() {
	// TODO Auto-generated destructor stub
}

