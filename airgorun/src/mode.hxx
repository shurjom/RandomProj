#ifndef _TESTSDL_GAME_MODE_HXX
#define _TESTSDL_GAME_MODE_HXX

extern "C" {
#include <SDL.h>
#include <SDL_ttf.h>
}

class mode {
public:
	/**
	 * \param new_mode
	 *   Set to a new mode to be pushed onto the stack.
	 * \returns
	 *   true to pop this mode from the mode stack.
	 */
	virtual bool processEvents(SDL_Event *event, mode *& new_mode) = 0;
	virtual void animate() = 0;
	virtual void render(SDL_Renderer *ren, TTF_Font *font) = 0;
	virtual ~mode();
};

#endif /* _TESTSDL_GAME_MODE_HXX */
