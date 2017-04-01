/*
 * object_block.hxx
 *
 *  Created on: May 25, 2015
 *      Author: Kristofer
 */

#ifndef SRC_OBJECT_BLOCK_HXX_
#define SRC_OBJECT_BLOCK_HXX_

#include "block.hxx"
#include "object.hxx"

#include <functional>
#include <vector>

class object_block
: public block {
public:
	object_block(SDL_Renderer *ren, int block_side_length, int grid_x, int grid_y);
	object_block(object_block&& that);
	void citerate(std::function<void(const object&)> visit);
	void draw(SDL_Renderer *ren, const camera& display_camera) const;
	void iterate(std::function<void(object&)> visit);
	object_block& operator=(object_block&& that);
	virtual ~object_block();
	std::vector<std::unique_ptr<object> > objects;
};

#endif /* SRC_OBJECT_BLOCK_HXX_ */
