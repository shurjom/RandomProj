/*
 * universe.cxx
 *
 *  Created on: Apr 3, 2015
 *      Author: Kristofer
 */

#include "universe.hxx"
#include <cmath>
#include <iostream>

static void circleCollisionResponse(object& obj1, object& obj2, double& circleOne_new_velocity_x, double& circleOne_new_velocity_y);

universe::universe(SDL_Renderer *ren, player& player)
: myPlayer(player)
{
}

static bool circlesAreColliding(double x1, double y1, double r1, double x2, double y2, double r2, double& distance)
{
	distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	return distance <= (r1 + r2);
}

static bool circlesAreCollidingByDistance(const object& obj1, const object& obj2, double distance) {
        return distance <= (obj1.getRadius() + obj2.getRadius());
}

static void normalize_vector(double& x, double& y)
{
	auto current_magnitude = sqrt(pow(x, 2) + pow(y, 2));
	x /= current_magnitude;
	y /= current_magnitude;
}

// ojb2 should be planet. Caller must verify that the object are
// within the gravitational_radius_of_influence of each other.
static void gravity(object& obj1, object& obj2, double distance) {
	// Has attractiveness
	if (obj1.getAttractive() || obj2.getAttractive()) {
		double force = universe::G*obj1.getMass()*obj2.getMass()/pow(distance, 2);

		// norm_x/norm_y are direction towards obj1 from obj2's perspective
		double norm_x = obj1.getX() - obj2.getX();
		double norm_y = obj1.getY() - obj2.getY();
		normalize_vector(norm_x, norm_y);
		if (obj1.getAttractive())
			obj2.applyForce(force, norm_x, norm_y);
		if (obj2.getAttractive())
			obj1.applyForce(force, -norm_x, -norm_y);
	}
}

static void collision(object& obj1, object& obj2, double distance) {
        if (circlesAreCollidingByDistance(obj1, obj2, distance))
        {
                double obj1_new_vel_x, obj1_new_vel_y;
                double obj2_new_vel_x, obj2_new_vel_y;
                circleCollisionResponse(obj1, obj2, obj1_new_vel_x, obj1_new_vel_y);
                circleCollisionResponse(obj2, obj1, obj2_new_vel_x, obj2_new_vel_y);
                obj1.setVelocity(obj1_new_vel_x, obj1_new_vel_y);
                obj2.setVelocity(obj2_new_vel_x, obj2_new_vel_y);
        }
}

static void interact(object& obj1, object& obj2) {
        double distance;

        // Verify that things are allowed to interact with each
        // other. Things may only interact when they are within the
        // gravitational_radius_of_influence.
        if (circlesAreColliding(obj1.getX(), obj1.getY(), std::max(obj1.getGROI(), obj2.getGROI()), obj2.getX(), obj2.getY(), 0, distance))
        {
                gravity(obj1, obj2, distance);
                collision(obj1, obj2, distance);
        }
}

void universe::animate() {
	objects.animate(interact, myPlayer);
	myPlayer.animate();
}

void universe::draw(SDL_Renderer *ren, camera& displayCamera){
	objects.draw(ren, displayCamera);
	// Draw player
	myPlayer.draw(ren, displayCamera);
}

universe::~universe() {
}

static double magnitude(double x, double y) {
        return sqrt(pow(x, 2) + pow(y, 2));
}

static double angle(double x, double y) {
        if (fabs(x) < 0.0001)
                return y > 0 ? 90 : 270;
        else
                return fmod(180 * atanf(y/x) / M_PI + (x > 0 ? 0 : 180) + 360, 360);
}

// Code copied from pogowars
static void circleCollisionResponse(object& obj1, object& obj2, double& circleOne_new_velocity_x, double& circleOne_new_velocity_y) {
	
	//Setting up variables to put into the weird equation found from wikipedia at http://en.wikipedia.org/wiki/Elastic_collision at 2 and three-dimensional
        double obj1_xVel, obj1_yVel; obj1.getVelocity(obj1_xVel, obj1_yVel);
        double obj2_xVel, obj2_yVel; obj2.getVelocity(obj2_xVel, obj2_yVel);
	double speed1 = magnitude(obj1_xVel, obj1_yVel);
	double speed2 = magnitude(obj2_xVel, obj2_yVel);
	double angle1 = angle(obj1_xVel, obj1_yVel)/180*M_PI;
	double angle2 = angle(obj2_xVel, obj2_yVel)/180*M_PI;
	double m1 = obj1.getMass();
	double m2 = obj2.getMass();
	double contactAngle = angle(obj2.getX() - obj1.getX(), obj2.getY() - obj1.getY())/180*M_PI;
	
	double topPartOverBottomPart = (speed1 * cos(angle1 - contactAngle) * (m1-m2) + 2*m2*speed2 * cos(angle2 - contactAngle)) / (m1 + m2);
	double sidePartForX = speed1 * sin(angle1 - contactAngle) * cos(contactAngle + M_PI / 2);
	double sidePartForY = speed1 * sin(angle1 - contactAngle) * sin(contactAngle + M_PI / 2);

	// Put in velocities
        circleOne_new_velocity_x = topPartOverBottomPart * cos(contactAngle) + sidePartForX;
        circleOne_new_velocity_y = topPartOverBottomPart * sin(contactAngle) + sidePartForY;

	//Move away if overlapping
        auto contactDistance = obj1.getRadius() + obj2.getRadius();
        obj2.setPosition(
                obj1.getX() + cos(contactAngle) * contactDistance,
                obj1.getY() + sin(contactAngle) * contactDistance);
}
