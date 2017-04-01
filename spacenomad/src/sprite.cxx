extern "C" {
#include <math.h>
#include <SDL.h>
}

#include "sprite.hxx"

#include <fstream>
#include <string>
#include <iostream>

sprite::sprite(std::string path, SDL_Texture* texture, int ticksPerFrame, int posX, int posY)
: height(0)
, width(0)
, numberOfFrames(0)
{
	
	std::ifstream ifs (("images/"+ path + ".sprite").c_str(), std::ifstream::in);
	
	currentFrame = 0;
	ticksSinceLastFrame = 0;
	this->texture = texture;
	this->ticksPerFrame = ticksPerFrame;
	char c;
	ifs >> c;
	ifs >> numberOfFrames;
	ifs >> width;
	ifs >> height;
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, NULL);
	
	std::cerr<<numberOfFrames<<", "<<width<<", "<<height<<"."<<std::endl;
	
	this->posX = posX;
	this->posY = posY;
	
}

void sprite::changePos(int x, int y) {
	
	posX = x;
	posY = y;
}

void sprite::chooseFrame(int frameChosen){
	currentFrame = frameChosen;
}

void sprite::nextTick(){
	ticksSinceLastFrame++;
	if (ticksSinceLastFrame >= ticksPerFrame){
		currentFrame++;
		if (currentFrame >= numberOfFrames)
			currentFrame = 0;
		ticksSinceLastFrame = 0;
	}
}

void sprite::render(SDL_Renderer *ren/*, camera *my_camera*/) {
	//destination rectangle or where it is rendered on the screen
	SDL_Rect dst;
	dst.x = posX;
	dst.y = posY;
	dst.w = width;
	dst.h = height;
	//source rectangle or which part of the texture we are rendering
	SDL_Rect src;
	int linearX = currentFrame * width;
	src.x = linearX % textureWidth;
	src.y = height * (linearX/textureWidth);
	src.w = width;
	src.h = height;
	
	/*my_camera->transform(&dst);*/
	SDL_RenderCopyEx(ren, texture, &src, &dst, /*angle*/ 0, NULL, SDL_FLIP_NONE);
}

sprite::~sprite() {
}
