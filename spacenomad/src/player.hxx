/*
 * player.hxx
 *
 *  Created on: Mar 20, 2015
 *      Author: Kristofer
 */

#ifndef SRC_PLAYER_HXX_
#define SRC_PLAYER_HXX_

#include "object.hxx"

class player : public object {
public:
	player(SDL_Renderer *ren, const char *textureFilename);
	bool up;
	bool down;
	bool left;
	bool right;
	bool shift;
	bool space;
	virtual void animate();
	virtual ~player();
private:
};

#endif /* SRC_PLAYER_HXX_ */
