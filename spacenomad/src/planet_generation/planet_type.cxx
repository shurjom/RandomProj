/*
 * planet_type.cxx
 *
 *  Created on: Jul 31, 2015
 *      Author: Kristofer
 */

#include "planet_type.hxx"

namespace spacenomad {

planet_type::color::color(int r, int g, int b, int a)
: r(r)
, g(g)
, b(b)
, a(a) {
}

planet_type::planet_type(const std::string& name)
: features()
, name(name) {
}

void
planet_type::add_feature(feature_type type, const color& color_min, const color& color_max, float sharpness) {
	features.push_back({ type, color_min, color_max, sharpness });
}

const std::vector<planet_type::feature>&
planet_type::get_features() const {
	return features;
}

planet_type::~planet_type() {
}

} /* namespace spacenomad */
