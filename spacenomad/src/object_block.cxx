/*
 * object_block.cxx
 *
 *  Created on: May 25, 2015
 *      Author: Kristofer
 */

#include "object_block.hxx"
#include "planet.hxx"

object_block::object_block(SDL_Renderer *ren, int block_side_length, int grid_x, int grid_y)
: block(block_side_length, grid_x, grid_y)
{
	objects.push_back(std::unique_ptr<planet>(new planet(ren, 300, 400, 8, -9, 2, 1)));
}

object_block::object_block(object_block&& that)
: block(std::move(that))
, objects(std::move(that.objects))
{
}

void
object_block::citerate(std::function<void(const object&)> visit)
{
	for (auto iter = objects.cbegin(); iter != objects.cend(); iter++)
		visit(*iter->get());
}

void
object_block::draw(SDL_Renderer *ren, const camera& display_camera) const {
	for (auto citer = objects.cbegin(); citer != objects.cend(); citer++) {
		const auto& o = *citer->get();
		o.draw(ren, display_camera);
	}
}

void
object_block::iterate(std::function<void(object&)> visit)
{
	for (auto iter = objects.begin(); iter != objects.end(); iter++)
		visit(*iter->get());
}

object_block&
object_block::operator=(object_block&& that) {
	objects = std::move(that.objects);
	return *this;
}

object_block::~object_block() {
}

