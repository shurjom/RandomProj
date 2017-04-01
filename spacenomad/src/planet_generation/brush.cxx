/*
 * brush.cxx
 *
 *  Created on: Jun 24, 2015
 *      Author: Kristofer
 *
 *  Will create a random brush for making textures on planets.
 */


// Step one, random dotted brush, work on different opacity also.

/*
 * Things to randomize
 * Size
 * Number of "dots", reacts to size
 * Type of dots, reacts to Number of dots
 * Per dot, shape, size, opacity, position
 */
#include "brush.hxx"

#include <iostream>
#include <vector>

namespace spacenomad {

// Assumes that coordinates are pixels or whatever that means
void
brush::fill_circle(SDL_Renderer *ren, const SDL_Rect& bounds, bool inverted) {
	// Using ellipse standard form (x - h)^2/a^2 + (y-k)^2/b^2 = 1.
	// Draw a pixel if its center satisfies <= 1.
	auto a = bounds.w / 2.0;
	auto h = bounds.x + a;
	auto b = bounds.h / 2.0;
	auto k = bounds.y + b;
	auto y = bounds.y + b;

	for (auto x = bounds.x; x < bounds.x + bounds.w/2 + 1; x++) {
			// Measure from the center of each pixel rather than something else.
			double pix_center_x = x + 0.5;
			double pix_center_y;
			do {
				y--;
				pix_center_y = y + 0.5;
			} while ((pix_center_x - h)*(pix_center_x - h) / (a*a) + (pix_center_y - k)*(pix_center_y - k) / (b*b) <= 1);
			y++;
			auto x_mirrored = bounds.x + bounds.w - (x - bounds.x);
			if (inverted)
			{
				SDL_RenderDrawLine(ren, x, y, x, bounds.y);
				SDL_RenderDrawLine(ren, x, bounds.y + bounds.h - (y - bounds.y), x, bounds.y + bounds.h);
				SDL_RenderDrawLine(ren, x_mirrored, y, x_mirrored, bounds.y);
				SDL_RenderDrawLine(ren, x_mirrored, bounds.y + bounds.h - (y - bounds.y), x_mirrored, bounds.y + bounds.h);
			}
			else
			{
				SDL_RenderDrawLine(ren, x, y, x,  bounds.y + bounds.h - (y - bounds.y));
				SDL_RenderDrawLine(ren, x_mirrored, y, x_mirrored,  bounds.y + bounds.h - (y - bounds.y));
			}

			continue;
			//std::cerr << x << "," << y << ": " << ((pix_center_x - h)*(pix_center_x - h) / (a*a) + (pix_center_y - k)*(pix_center_y - k) / (b*b)) << std::endl;
		}
}

brush::brush(std::default_random_engine & random_engine) {

	// Test to see if randomness works
	std::cout << "Making brush" << std::endl;
	std::uniform_int_distribution<int> randomNumberDist(0,10000);
	std::cout << "Testing the randomness in brush " << randomNumberDist(random_engine) << std::endl;

	// Size of brush
	std::uniform_int_distribution<int> distr_randSize(5, 300);
	auto size = distr_randSize(random_engine);

	// Make surface
	surface = std::move(space_nomad_SDL_Surface_unique_ptr(SDL_CreateRGBSurface(0, size, size, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000)));
	space_nomad_SDL_Renderer_unique_ptr surface_ren(SDL_CreateSoftwareRenderer(surface.get()));

	// Amount of specs in brush
	std::uniform_int_distribution<int> distr_randSpecs(1, 5);

	// Set up to make specs
	auto numSpecs = distr_randSpecs(random_engine);
	SDL_Rect dst;

	// Set up base random color
	std::uniform_int_distribution<int> distr_randColor(0, 255);
	// Don't make invisible/barely visible brushes: constrain the alpha to >15.
	std::uniform_int_distribution<int> distr_randAlpha(16, 255);
	std::vector<int> rgb;
	for (int i = 0; i < 3; i++) {
		rgb.push_back(distr_randColor(random_engine));
	}
	auto alpha = distr_randAlpha(random_engine);

	// Making the specs
	for (int i = 0; i < numSpecs; i++) {

		// Location and size
		std::uniform_int_distribution<int> distr_length(1, size/2);
		dst.w = distr_length(random_engine);
		dst.h = distr_length(random_engine);
		std::uniform_int_distribution<int> distr_pos_x(dst.w, size - dst.w);
		std::uniform_int_distribution<int> distr_pos_y(dst.h, size - dst.h);
		dst.x = distr_pos_x(random_engine);
		dst.y = distr_pos_y(random_engine);

		// Random color
		SDL_SetRenderDrawColor(surface_ren.get(), rgb[0], rgb[1], rgb[2], alpha);
		//TODO make the color vary slightly

		fill_circle(surface_ren.get(), dst);
	}
}


brush::brush(brush&& orig) {
	surface = std::move(orig.surface);
}


brush::~brush() {
}

}; /* namespace spacenomad */
