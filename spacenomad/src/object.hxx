/*
 * object.h
 *
 *  Created on: Mar 20, 2015
 *      Author: Kristofer
 */

#ifndef SRC_OBJECT_HXX_
#define SRC_OBJECT_HXX_

#include "main.hxx"
extern "C" {
#include <SDL.h>
}

#include "camera.hxx"

class object {
public:
	object(
		SDL_Renderer *ren,
		const char *texture,
		double x = 0,
		double y = 0,
		double xVel = 0,
		double yVel = 0,
		double angular = 0,
		double mass = 0);
	object(
		SDL_Renderer *ren,
		space_nomad_SDL_Surface_unique_ptr&& surface,
		double x = 0,
		double y = 0,
		double xVel = 0,
		double yVel = 0,
		double angular = 0,
		double mass = 0);
	object(
			object&& src);
	virtual void animate();
	void draw(SDL_Renderer *ren, const camera& displayCamera) const;
	void angularVel_dampening(double dampening);
	void movement_dampening(double dampening);
	void considerCamera(camera& displayCamera, int clearFactor);
	bool getAttractive() const { return attractive;};
	double getX() const { return x; };
	double getY() const { return y; };
        void setPosition(double x, double y) { this->x = x; this->y = y; };
	int getRadius() const { return radius; };
	double getGROI() const { return gravitational_radius_of_influence; };
	double getMass() const { return mass; };
        void getVelocity(double& xVel, double& yVel) const { xVel = this->xVel; yVel = this->yVel; };
        void setVelocity(double xVel, double yVel) { this->xVel = xVel; this->yVel = yVel; };
	/**
	 * \param x
	 *   Normalized x component of force vector.
	 * \param y
	 *   Normalized y component of force vector.
	 */
	void applyForce(double magnitude, double x, double y);
	virtual ~object();
protected:
	double x;
	double y;
	double xVel;
	double yVel;
	double facingDirection;
	double angularVel;
	bool attractive;
	double mass;
	int radius;
	space_nomad_SDL_Texture_unique_ptr texture;
	double gravitational_radius_of_influence;
};

#endif /* SRC_OBJECT_HXX_ */
