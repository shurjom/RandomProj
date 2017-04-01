/*
 * game_mode.cxx
 *
 *  Created on: Jan 5, 2015
 *      Author: Kristofer
 */


#include "game_mode.hxx"

game_mode::game_mode()
{
}


bool game_mode::processEvents(SDL_Event *event, mode *& new_mode)
{
	return false;
}
void game_mode::animate()
{
}
void game_mode::render(SDL_Renderer *ren, TTF_Font *font)
{
}

game_mode::~game_mode()
{
}
