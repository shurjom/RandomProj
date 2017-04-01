/*
 * game_mode.hxx
 *
 *  Created on: Jan 5, 2015
 *      Author: Kristofer
 */

#ifndef SRC_GAME_MODE_HXX_
#define SRC_GAME_MODE_HXX_


#include "mode.hxx"

class game_mode
: public mode
{
public:
	game_mode();
	virtual bool processEvents(SDL_Event *event, mode *& new_mode);
	virtual void animate();
	virtual void render(SDL_Renderer *ren, TTF_Font *font);
	~game_mode();
};


#endif /* SRC_GAME_MODE_HXX_ */
