/*
 * planet_generation_mode.cxx
 *
 *  Created on: Jun 23, 2015
 *      Author: Kristofer
 */

#include "main.hxx"
#include "camera.hxx"
#include "player.hxx"
#include "star_field.hxx"
#include "universe.hxx"
#include "planet_generation/brush.hxx"
#include "planet_generation/planet_generation_mode.hxx"
#include "planet_generation/planet_generator.hxx"

#include <ctime>
#include <functional>
#include <iostream>
#include <array>

namespace spacenomad {

planet_generation_mode::planet_generation_mode(SDL_Renderer *ren)
: meBraggingAbout4k(loadTexture(ren, "planet_generation_mode/This is 4k.png"))
, random_engine(time(NULL))
, planet(std::move(planet_generator().generate(ren, prod_random_engine(random_engine)))) {
}


std::default_random_engine&
planet_generation_mode::prod_random_engine(std::default_random_engine& engine) {
	// So, uniform_int_distribution treats very large numbers that
	// differ only in the hundreds place or below as the
	// same. Because our time() values are very large and near
	// each other, and because the random_engine() yields the seed
	// value as the first number, we have to iterate past that to
	// make future uses of the random_engine() happy…	random_engine();
	engine();
	return engine;
}

bool planet_generation_mode::processEvents(SDL_Event *event, main_class& main) {
	switch (event->type)
	{
	case SDL_KEYDOWN:
		if (event->key.repeat)
			break;

		switch (event->key.keysym.sym)
		{
		// case Up
		case SDLK_BACKSPACE:
			return true;
		}
	}
	return false;
}

void planet_generation_mode::animate() {

}

void planet_generation_mode::render(SDL_Renderer *ren, camera& displayCamera, TTF_Font *font) {
	SDL_Rect dst;

	// This Renders the image about me bragging about 4k
	dst.x = 0;
	dst.y = 0;
	SDL_QueryTexture(meBraggingAbout4k.get(), NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, meBraggingAbout4k.get(), NULL, &dst);

	SDL_SetRenderDrawColor(ren, 128, 0, 255, 0);
	dst.x = 200;
	dst.y = 300;
	dst.h = 400;
	dst.w = 500;
	SDL_RenderDrawRect(ren, &dst);
	SDL_SetRenderDrawColor(ren, 255, 0, 128, 0);
	brush::fill_circle(ren, dst);

	// Build a palette
	auto brush_s = 48;
	std::vector<space_nomad_SDL_Surface_unique_ptr> palette;
	for (int r = 0; r <= 255; r += 255)
		for (int g = 0; g <= 255; g += 255)
			for (int b = 0; b <= 255; b += 255)
			{
				// You have to specify the rmask, etc. ridiculousness with the 0x00000ff00 and whatnot
				// manually otherwise you don't get an alpha channel
				auto myBrush = space_nomad_SDL_Surface_unique_ptr(
						SDL_CreateRGBSurface(0, brush_s, brush_s, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000));
				{
					auto brush_ren = space_nomad_SDL_Renderer_unique_ptr(
							SDL_CreateSoftwareRenderer(myBrush.get()));
					// Do something fancy to render things to the brush itself.
					// In SDL_SetRenderDrawColor(), the alpha value of 0 means fully transparent.
					// 255 means fully opaque. The 128 we have here is half-transparent.
					SDL_SetRenderDrawColor(brush_ren.get(), r, g, b, 128);
					// Draw everything in it to a color duh.
					SDL_RenderClear(brush_ren.get());

					// Draw a solid circle in the brush.
					dst.x = 0;
					dst.y = 0;
					dst.w = brush_s;
					dst.h = brush_s;
					SDL_SetRenderDrawColor(brush_ren.get(), 255-r, 255-g, 255-b, 255);
					brush::fill_circle(brush_ren.get(), dst);

					// Now, to demonstrate how to draw with alpha blending disabled,
					// I am going to draw a circle ridiculously with full alpha but
					// blending disabled to sort of create a hole in the brush.
					SDL_SetRenderDrawBlendMode(brush_ren.get(), SDL_BLENDMODE_NONE);
					// Here's my fully transparent brush. The RGB values, of course,
					// don't matter because the alpha value is 0 (full transparently).
					SDL_SetRenderDrawColor(brush_ren.get(), 0, 0, 0, 0);
					dst.w /= 2;
					dst.h /= 2;
					dst.x += dst.w;
					dst.y += dst.h;
					brush::fill_circle(brush_ren.get(), dst);
				}
				palette.push_back(std::move(myBrush));
			}

	// Draw the palette
	int palette_y = 40;
	for (auto brush_iter = palette.begin(); brush_iter != palette.end(); brush_iter++)
	{
		dst.x = 20;
		dst.y = palette_y;
		dst.w = 32;
		dst.h = 32;
		//displayCamera.transform(&dst);
		SDL_RenderCopy(ren, createTexture(ren, *brush_iter).get(), NULL, &dst);
		palette_y += brush_s + 16;
	}
	SDL_SetRenderDrawColor(ren, 127, 127, 127, 0);
	// Make all the testing stuff go away.
	SDL_RenderClear(ren);
	displayCamera.clear();
	this->planet.considerCamera(displayCamera, 1);
	displayCamera.calculateTransforms();
	this->planet.draw(ren, displayCamera);
}

planet_generation_mode::~planet_generation_mode() {
}


}; /* namespace spacenomad */
