/*
 * game_mode.hxx
 *
 *  Created on: Jan 5, 2015
 *      Author: Kristofer
 */

#ifndef SRC_GAME_MODE_HXX_
#define SRC_GAME_MODE_HXX_

#include "camera.hxx"
#include "main.hxx"
#include "mode.hxx"
#include "player.hxx"
#include "star_field.hxx"
#include "universe.hxx"

class game_mode
: public mode
{
public:
	game_mode(SDL_Renderer *ren, SDL_Window *win);
	virtual bool processEvents(SDL_Event *event, main_class& main);
	virtual void animate();
	virtual void render(SDL_Renderer *ren, camera& displayCamera, TTF_Font *font);
	~game_mode();
private:
	// Player
	player myPlayer;
	player myStaticPlayer;
	universe myUniverse;
	star_field stars;
	bool zoom;
	SDL_Window *win;
};


#endif /* SRC_GAME_MODE_HXX_ */
