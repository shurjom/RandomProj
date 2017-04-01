/*
 * menu_mode.cxx
 *
 *  Created on: Jan 3, 2015
 *      Author: Kristofer
 */



#include "game_mode.hxx"
#include "main.hxx"
#include "menu_mode.hxx"

static bool menu_option_start(mode *&new_mode)
{
	new_mode = new game_mode();
	return false;
}

static bool menu_option_credits(mode *&new_mode)
{
	new_mode = NULL;
	return false;
}

static bool menu_option_quit(mode *&new_mode)
{
	return true;
}

menu_mode::menu_mode(SDL_Renderer *ren)
: tex_menu_back(loadTexture(ren, "menu/menu_back.png"))
, tex_choose_bar(loadTexture(ren, "menu/choose_bar.png"))
, selection_index(0) // menu bar selection
, menu_options({
			menu_option(menu_option_start),
			menu_option(menu_option_credits),
			menu_option(menu_option_quit),
	})
{
}

bool menu_mode::processEvents(SDL_Event *event, mode *& new_mode)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		if (event->key.repeat)
			break;

		switch (event->key.keysym.sym)
		{
		// case Up
		case SDLK_UP:
		case SDLK_w:
			if (selection_index > 0)
				selection_index--;
			break;
		// case down
		case SDLK_DOWN:
		case SDLK_s:
			if (selection_index < AIRGO_ARRAY_SIZE(menu_options) - 1)
				selection_index++;
			break;

		// case right
		case SDLK_RIGHT:
		case SDLK_d:

		// case enter or space
		case SDLK_KP_ENTER:
		case SDLK_RETURN:
		case SDLK_KP_SPACE:
			return menu_options[selection_index].do_thing(new_mode);
		}
	}

	return false;
}

void
menu_mode::animate()
{
}

void
menu_mode::render(SDL_Renderer *ren, TTF_Font *font)
{
	SDL_Rect dst;
	// Menu back
	dst.x = dst.y = 0;
	SDL_QueryTexture(tex_menu_back.get(), NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex_menu_back.get(),NULL, &dst);
	// CHoose bar
	dst.x = 294;
	SDL_QueryTexture(tex_choose_bar.get(), NULL, NULL, &dst.w, &dst.h);
	dst.y = 688 + selection_index * (dst.h - 11);
	SDL_RenderCopy(ren, tex_choose_bar.get(), NULL, &dst);
}

menu_mode::~menu_mode()
{
}
