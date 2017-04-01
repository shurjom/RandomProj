/*
 * planet_generator.hxx
 *
 *  Created on: Jul 31, 2015
 *      Author: Kristofer
 */

#ifndef SRC_PLANET_GENERATION_PLANET_GENERATOR_HXX_
#define SRC_PLANET_GENERATION_PLANET_GENERATOR_HXX_

#include "planet.hxx"
#include "planet_generation/planet_type.hxx"

#include <random>
#include <vector>

namespace spacenomad {

class planet_generator {
public:
	planet_generator();
	planet generate(SDL_Renderer *ren, std::default_random_engine& random_engine) const;
	virtual ~planet_generator();
private:
	std::vector<planet_type> planet_types;
};

} /* namespace spacenomad */

#endif /* SRC_PLANET_GENERATION_PLANET_GENERATOR_HXX_ */
