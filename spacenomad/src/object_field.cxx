/*
 * objectfield.cxx
 *
 *  Created on: May 25, 2015
 *      Author: Kristofer
 */

#include "object_field.hxx"
#include "planet.hxx"

static const int object_field_block_side_length = 4096;

object_field_layer::object_field_layer()
: field_layer(object_field_block_side_length) {
}

void
object_field::animate(void(*interact)(object& obj1, object& obj2), player& myPlayer)
{
	objects_layer.animate(interact, myPlayer);
}

void
object_field_layer::add_block(std::default_random_engine& random_engine, int grid_x, int grid_y, SDL_Renderer *ren)
{
	object_blocks.push_back(
			std::move(
					object_block(
							ren,
							get_block_side_length(),
							grid_x,
							grid_y)));
}

void
object_field_layer::citerate(std::function<bool(const block&)> visit)
{
	for (size_t i = object_blocks.size(); i > 0; i--)
		if (visit(object_blocks[i - 1]))
			object_blocks.erase(object_blocks.begin() + i - 1);
}

void
object_field_layer::iterate_objects(std::function<void(object&)> visit)
{
	for (auto block_iter = object_blocks.begin(); block_iter != object_blocks.cend(); block_iter++)
		block_iter->iterate(visit);
}

void
object_field_layer::animate(void(*interact)(object&, object&), player& myPlayer) {
	// In the future, do objects within each block, then objects between
	// adjacent blocks. For now, just be incorrect and only have things
	// inside a block interact.
	for (auto block_iter = object_blocks.begin(); block_iter != object_blocks.end(); block_iter++)
	{
		for (auto obj_iter = block_iter->objects.begin(); obj_iter != block_iter->objects.end(); obj_iter++) {
			auto obj = obj_iter->get();
			for (auto obj2_iter = obj_iter + 1 ; obj2_iter != block_iter->objects.end(); obj2_iter++) {
				auto obj2 = obj2_iter->get();
				interact(*obj, *obj2);
			}
			interact(*obj, myPlayer);
			obj->animate();
		}
	}
}

object_field_layer::~object_field_layer() {
}

object_field::object_field()
: field(object_field_block_side_length) {
}

void
object_field::iterate(std::function<void(field_layer&)> visit) {
	visit(objects_layer);
}

object_field::~object_field() {
}
