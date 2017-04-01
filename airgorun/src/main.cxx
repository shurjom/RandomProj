#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <locale>
#include <memory>
#include <stack>
#include <sstream>
#include <string>
#include <streambuf>

extern "C" {
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
}

#include "main.hxx"
#include "menu_mode.hxx"
#include "mode.hxx"

static Uint32 tickTimerCallback(Uint32 interval, void *param);

class main_class
{
private:
	std::stack<mode *> modes;
	void push_mode(mode *mode)
	{
		modes.push(mode);
	}
	void delete_modes()
	{
		while (!modes.empty())
			delete_top_mode();
	}
	void delete_top_mode()
	{
		delete modes.top();
		modes.pop();
	}
public:
	int main_method(int argc, char *argv[])
	{
	/* http://twinklebear.github.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world/ */

	// Starts SDL
	if (SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Loads the font loading system # seriously # end world hunger games
	if (TTF_Init()==-1)
	{
		std::cout << "TTF_Init: " << TTF_GetError() << std::endl;
		return 1;
	}

	TTF_Font *font;
	font=TTF_OpenFont("VeraMono.ttf", 90);
	if (!font)
	{
		std::cout <<"TTF_OpenFont:" << TTF_GetError() << std::endl;
		return 1;
	}

	// Create window
	SDL_Window *win =  SDL_CreateWindow("Airgo Run", 100, 100, 1280, 1024, SDL_WINDOW_RESIZABLE);
	if (win == NULL){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Loads the icon
	auto ico = loadSurface("favicon.png");
	SDL_SetWindowIcon(win, ico.get());

	//Creating a Renderer: Using hardware accelerated rendering and with vsync.
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL)
	{
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		std::cerr << "Trying without VSYNC" << std::endl;
		ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
		if (!ren)
		{
			std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
			std::cerr << "Falling back to low-quality (no alpha blending, game may be unplayable) renderer." << std::endl;
			ren = SDL_CreateRenderer(win, -1, 0);
			if (!ren)
			{
				std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
				std::cerr << "Well, I need a renderer and I can’t even get a low-quality one. Giving up." << std::endl;
				return 1;
			}
		}
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Setting timer
	// Set up a user event for detecting timer events.
	// See http://wiki.libsdl.org/SDL_AddTimer
	Uint32 tickSdlEventCode = SDL_RegisterEvents(1);
	//60 fps
	int tickMilliseconds = 1000/60;
	//double tickSeconds = tickMilliseconds / 1000.0;
	SDL_TimerID tickTimerID = SDL_AddTimer(tickMilliseconds, tickTimerCallback, &tickSdlEventCode);
	if (!tickTimerID)
	{
		std::cerr << "SDL_AddTimer() Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//Start the things needed to run the game
	SDL_Event event;
	SDL_ShowCursor(SDL_DISABLE);

	// Initial mode: the top level main menu
	modes.push(new menu_mode(ren));

	while (!modes.empty())
	{
		//Set up needed variables
		bool animate = true;
		bool redraw = true;

		while(!modes.empty() && SDL_WaitEvent(&event)){
			do
			{
				// call mode-specific event handler
				mode *new_mode = NULL;
				if (modes.top()->processEvents(&event, new_mode))
					delete_top_mode();
				if (new_mode)
					modes.push(new_mode);
				if (modes.empty())
					break;
				// run generic event handlers
				switch (event.type)
				{
				case SDL_QUIT:
					delete_modes();
					break;
				case SDL_WINDOWEVENT:
					switch (event.window.event)
					{
					case SDL_WINDOWEVENT_RESIZED:
						redraw = true;
						break;
					case SDL_WINDOWEVENT_CLOSE:
						delete_modes();
						break;
					}
					break;
				case SDL_KEYDOWN:
					if (event.key.repeat)
						break;
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						delete_modes();
						break;
					}
					break;
				case SDL_USEREVENT:
					if (event.user.type == tickSdlEventCode)
						animate = true;
					break;
				}


			// Eat all of the other events while we're at it.
			} while (!modes.empty() && SDL_PollEvent(&event));
			if (modes.empty())
				break;

			if (animate)
			{
				modes.top()->animate();

				// After updating animation stuffs, mark that we're ready for a redraw
				redraw = true;
				animate = false;
			}

			if (redraw)
			{
				SDL_RenderClear(ren);
				modes.top()->render(ren, font);

				SDL_RenderPresent(ren);
				redraw = false;
			}
		}
	} /* while (!modes.empty()) */

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ /end player one game
	// Cleanup
	SDL_RemoveTimer(tickTimerID);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();

	/*system("PAUSE");*/
	return EXIT_SUCCESS;
}
	~main_class()
	{
		delete_modes();
	}
};

int main(int argc, char *argv[])
{
	main_class m;
	return m.main_method(argc, argv);
}

// http://stackoverflow.com/a/19054280/2948122
void
airgo_SDL_Texture_deleter::operator()(SDL_Texture *texture)
{
	SDL_DestroyTexture(texture);
}
// Load a surface
airgo_SDL_Surface_unique_ptr loadSurface(const char *filename)
{
	SDL_Surface *bmp = IMG_Load(("images/" + std::string(filename)).c_str());
	if (!bmp)
	{
			 std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
			 abort();
			 return airgo_SDL_Surface_unique_ptr();
	}
	return airgo_SDL_Surface_unique_ptr(bmp);
}
void
airgo_SDL_Surface_deleter::operator()(SDL_Surface *surface)
{
	SDL_FreeSurface(surface);
}
// Load surface then convert to texture and unload surface because of memory leaking. # it isn't that bad though
airgo_SDL_Texture_unique_ptr loadTexture(SDL_Renderer *ren, const char *filename)
{
	auto bmp = loadSurface(filename);

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp.get());
	if (!tex)
	{
			 std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			 abort();
			 return airgo_SDL_Texture_unique_ptr();
	}
	return airgo_SDL_Texture_unique_ptr(tex);
}

static Uint32 tickTimerCallback(Uint32 interval, void *param)
{
	Uint32 tickSdlEventCode = *(Uint32*)param;
	// http://wiki.libsdl.org/SDL_UserEvent
	SDL_Event event;
	SDL_zero(event);
	event.type = SDL_USEREVENT;
	event.user.type = tickSdlEventCode;
	SDL_PushEvent(&event);
	// Specify the amount of time to wait before running this event again.
	return interval;
}
