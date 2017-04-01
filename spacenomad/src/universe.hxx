/*
 * universe.hxx
 *
 *  Created on: Apr 3, 2015
 *      Author: Kristofer
 */

#ifndef SRC_UNIVERSE_HXX_
#define SRC_UNIVERSE_HXX_

#include "object_field.hxx"
#include "player.hxx"
#include <memory>
#include <vector>


class universe {
public:
	// Gravitational constant.
	static constexpr double G = 10;

	universe(SDL_Renderer *ren, player& player);
	void animate();
	void draw(SDL_Renderer *ren, camera& displayCamera);
	virtual ~universe();
private:
	object_field objects;
	player& myPlayer;
};

#endif /* SRC_UNIVERSE_HXX_ */
