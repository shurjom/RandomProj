#ifndef _TESTSDL_SPRITE_HXX
#define _TESTSDL_SPRITE_HXX

extern "C" {
#include <SDL.h>
}
#include <string>

class sprite {
public:
	sprite(std::string path, SDL_Texture* texture, int ticksPerFrame, int posX, int posY);
	/**
	 * \param frameChosen
	 *   0-based frame index.
	 */
	 
	void chooseFrame(int frameChosen);
	void changePos(int x, int y);
	void nextTick();
	void render(SDL_Renderer *ren/*, camera *my_camera*/);
	virtual ~sprite();
private:
	// minimum game area to display
	SDL_Texture *texture;
	
	int ticksPerFrame, height, width, numberOfFrames, posX, posY;
	// start at 0
    int currentFrame;
    int ticksSinceLastFrame;
	int textureWidth;
};

#endif 
 
