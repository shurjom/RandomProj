extern "C" {
#include <SDL.h>
}

#include "camera.hxx"

#include <algorithm>
#include <climits>
#include <cmath>

camera::camera(int displayWidth, int displayHeight)
: aspect((double)displayWidth/(double)displayHeight)
, displayHeight(displayHeight)
, leftMost(0)
, rightMost(0)
, topMost(0)
, bottMost(0)

// the transformation to apply
, scale(0)
, offsetX(0)
, offsetY(0)
{
}

void camera::clear()
{
	leftMost = INT_MAX;
	rightMost = INT_MIN;
	topMost = INT_MAX;
	bottMost = INT_MIN;
}


void camera::considerObject(int x, int y, int clearRadius)
{
	if (leftMost > x - clearRadius)
		leftMost = x - clearRadius;
	if (rightMost < x + clearRadius)
		rightMost = x + clearRadius;
	if (topMost > y - clearRadius)
		topMost = y - clearRadius;
	if (bottMost < y + clearRadius)
		bottMost = y + clearRadius;
}

void camera::calculateTransforms()
{
	// Fix aspect of the left/right/top/bottMost rectangle to match display aspect.
	// aspect_correction is the amount by which the width isn't wide enough
	int aspect_correction =
		aspect * (bottMost - topMost) // target width
		- (rightMost - leftMost); // actual object-encompassing width at the moment
	if (aspect_correction > 0)
	{
		rightMost += aspect_correction / 2;
		leftMost -= aspect_correction / 2 + aspect_correction % 2;
	}
	else
	{
		// apparently we're lacking in height instead.
		aspect_correction = (rightMost - leftMost) / aspect - (bottMost - topMost);
		bottMost += aspect_correction / 2;
		topMost -= aspect_correction / 2 + aspect_correction % 2;
	}

	scale = displayHeight / (double)(bottMost - topMost);
	offsetX = -leftMost;
	offsetY = -topMost;
}

camera camera::calculateParallax(double parallax_factor) const
{
        camera ret(*this);
        // Parallax about the center of the screen.
        ret.offsetX = (offsetX - get_display_width()/2) * parallax_factor + get_display_width()/2;
        ret.offsetY = (offsetY - displayHeight/2) * parallax_factor + displayHeight/2;
        return ret;
}

bool camera::transform(SDL_Rect *r) const
{
	r->x = (r->x + offsetX) * scale;
	r->y = (r->y + offsetY) * scale;
	r->w = r->w * scale;
	r->h = r->h * scale;

	// Calculate if the transformed object would appear on the display.
	// Be conservative because may be rotated.
	auto estimating_side_length = 2 * std::max(r->w, r->h);
	return r->x + estimating_side_length > 0
			&& r->y + estimating_side_length > 0
			&& r->x - estimating_side_length < get_display_width()
			&& r->y - estimating_side_length < displayHeight;
}

void camera::get_visible_area(SDL_Rect *r) const
{
	r->x = -offsetX;
	r->y = -offsetY;
	r->w = get_display_width()/scale;
	r->h = displayHeight/scale;
}

double camera::get_display_width() const
{
	return aspect*displayHeight;
}

camera::~camera()
{
}
