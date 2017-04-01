/*
 * objectfield.hxx
 *
 *  Created on: May 25, 2015
 *      Author: Kristofer
 */

#ifndef SRC_OBJECT_FIELD_HXX_
#define SRC_OBJECT_FIELD_HXX_

#include "camera.hxx"
#include "field.hxx"
#include "object.hxx"
#include "object_block.hxx"
#include "player.hxx"

#include <functional>

class object_field_layer
: public field_layer
{
public:
	object_field_layer();
	void add_block(std::default_random_engine& random_engine, int grid_x, int grid_y, SDL_Renderer *ren);
	void citerate(std::function<bool(const block&)> visit);
	void iterate_objects(std::function<void(object&)> visit);
	void animate(void(*interact)(object& obj1, object& obj2), player &myPlayer);
	virtual ~object_field_layer();
private:
	std::vector<object_block> object_blocks;
};

class object_field
: public field {
public:
	object_field();
	void animate(void(*interact)(object& obj1, object& obj2), player& myPlayer);
	void iterate(std::function<void(field_layer&)> visit);
	virtual ~object_field();
private:
	object_field_layer objects_layer;
};

#endif /* SRC_OBJECT_FIELD_HXX_ */
