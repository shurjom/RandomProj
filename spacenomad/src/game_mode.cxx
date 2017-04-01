/*
 * game_mode.cxx
 *
 *  Created on: Jan 5, 2015
 *      Author: Kristofer
 */


#include "game_mode.hxx"

game_mode::game_mode(
		SDL_Renderer *ren,
		SDL_Window *win)
: myPlayer(ren, "astronaut/space_suit.png")
, myStaticPlayer(ren, "astronaut/space_suit.png")
, myUniverse(ren, myPlayer)
, stars(ren)
, zoom(true)
, win(win)
{
	// Lock cursor to screen
	SDL_SetWindowGrab(win, SDL_TRUE);
}

bool game_mode::processEvents(SDL_Event *event, main_class& main)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym)
		{
		case SDLK_BACKSPACE:
			return true;
			// WASD
		case SDLK_w:
			myPlayer.up = true;
			break;
		case SDLK_s:
			myPlayer.down = true;
			break;
		case SDLK_a:
			myPlayer.left = true;
			break;
		case SDLK_d:
			myPlayer.right = true;
			break;
			//Shift
		case SDLK_LSHIFT:
			myPlayer.shift = true;
			break;

		case SDLK_LEFT:
			// Menuish stuff
			return true;
		case SDLK_z:
			zoom = !zoom;
			break;
		}
		break;
		case SDL_KEYUP:
			switch (event->key.keysym.sym)
			{
			// WASD
			case SDLK_w:
				myPlayer.up = false;
				break;
			case SDLK_s:
				myPlayer.down = false;
				break;
			case SDLK_a:
				myPlayer.left = false;
				break;
			case SDLK_d:
				myPlayer.right = false;
				break;
				//Shift
			case SDLK_LSHIFT:
				myPlayer.shift = false;
				break;
			}
			break;
	}
	return false;
}
void game_mode::animate()
{
	myUniverse.animate();
}
void game_mode::render(SDL_Renderer *ren, camera& displayCamera, TTF_Font *font)
{
	// Set the camera.
	displayCamera.clear();
	myPlayer.considerCamera(displayCamera, 6 * (zoom ? 1 : 10));
	displayCamera.calculateTransforms();
	// Draw stars
	stars.draw(ren, displayCamera);
	// Draw Universe
	myUniverse.draw(ren, displayCamera);
}

game_mode::~game_mode()
{
	SDL_SetWindowGrab(win, SDL_FALSE);
}
