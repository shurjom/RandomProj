/*
 * player.cxx
 *
 *  Created on: Mar 20, 2015
 *      Author: Kristofer
 */

#include "player.hxx"

#include <algorithm>
#include <iostream>
#include <cmath>

player::player(SDL_Renderer *ren, const char *textureFilename)
: object(ren, textureFilename)
, up(false)
, down(false)
, left(false)
, right(false)
, shift(false)
, space(false)
{
        mass = 1;
}


void player::animate() {
	// Turning
	const auto turnAccelSpeed = .2;
	if (left) {
		if (angularVel > 0) {
			object::angularVel_dampening(1.2);
		}
		angularVel -= turnAccelSpeed;
	}
	if (right) {
		if (angularVel < 0) {
			object::angularVel_dampening(1.2);
		}
		angularVel += turnAccelSpeed;
	}
	if (!right && !left){
		object::angularVel_dampening(1.2);
	}

	// Moving forwards
	const auto moveAccelSpeed = .3;
	if (up || down) {
		double xAccel = sin((180-facingDirection)/180*M_PI) * moveAccelSpeed;
		double yAccel = cos((180+facingDirection)/180*M_PI) * moveAccelSpeed;

		if (up) {
			xVel += xAccel;
			yVel += yAccel;
		}
		if (down) {
			xVel -= xAccel;
			yVel -= yAccel;
		}
	}
	object::animate();
}


player::~player() {
}

