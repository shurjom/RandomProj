/*
 * planet_type.hxx
 *
 *  Created on: Jul 31, 2015
 *      Author: Kristofer
 */

#ifndef SRC_PLANET_GENERATION_PLANET_TYPE_HXX_
#define SRC_PLANET_GENERATION_PLANET_TYPE_HXX_

#include <string>
#include <vector>

namespace spacenomad {

class planet_type {
public:
	struct color {
		int r, g, b, a;
		color(int r, int g, int b, int a = 255);
	};
	enum feature_type {
		FEATURE_TYPE_GROUNDSTUFF, // water, land, etc.
		FEATURE_TYPE_CLOUD,
		FEATURE_TYPE_RING,
	};
	struct feature {
		feature_type type;
		color color_min;
		color color_max;
		float sharpness;
	};
	planet_type(const std::string& name);
	void add_feature(
			feature_type type,
			const color& color_min,
			const color& color_max,
			float sharpness);
	const std::vector<feature>& get_features() const;
	const std::string get_name() const;
	virtual ~planet_type();
private:
	std::vector<feature> features;
	std::string name;
};

} /* namespace spacenomad */

#endif /* SRC_PLANET_GENERATION_PLANET_TYPE_HXX_ */
