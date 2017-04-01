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

#include "camera.hxx"
#include "main.hxx"
#include "menu_mode.hxx"
#include "mode.hxx"

static Uint32 tickTimerCallback(Uint32 interval, void *param);
int SDL_ToggleFS(SDL_Window* win, SDL_Renderer *ren);

int space_nomad_fps = 60;

void main_class::push_mode(mode *mode)
{
	modes.push(mode);
}
void main_class::delete_modes()
{
	while (!modes.empty())
		delete_top_mode();
}
void main_class::delete_top_mode()
{
	delete modes.top();
	modes.pop();
}
int main_class::main_method(int argc, char *argv[])
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
	win =  SDL_CreateWindow("Space Nomad", 100, 100, 1280, 1024, SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI);
	if (win == NULL){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}



	// Loads the icon
	auto ico = loadSurface("favicon.png");
	SDL_SetWindowIcon(win, ico.get());

	//Creating a Renderer: Using hardware accelerated rendering and with vsync.
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
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
	//space_nomad_fpds fps
	int tickMilliseconds = 1000/space_nomad_fps;
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

	// Customize the cursor
	space_nomad_SDL_Surface_unique_ptr cursor(loadSurface("cursor.png"));
	auto cursor_data = new Uint8[cursor->w*cursor->h/8]();
	auto cursor_mask = new Uint8[cursor->w*cursor->h/8]();
	SDL_LockSurface(cursor.get());
	for (int i = 0; i < cursor->w*cursor->h; i++) {
		Uint8 r, g, b, a;
		SDL_GetRGBA(
				*(Uint32*)((Uint8*)cursor->pixels + cursor->format->BytesPerPixel*i),
				cursor->format,
				&r, &g, &b, &a);
		Uint8 pixelBit = (1<<(7-(i%8)));
		cursor_data[i/8] |= r || !a ? 0 : pixelBit;
		cursor_mask[i/8] |= !a ? 0 : pixelBit;
	}
	SDL_UnlockSurface(cursor.get());
	auto customCursor = SDL_CreateCursor(cursor_data, cursor_mask, cursor->w, cursor->h, (cursor->w-2)/2, (cursor->h-2)/2);
	SDL_SetCursor(customCursor);

	delete [] cursor_data;
	delete [] cursor_mask;

	// Initial mode: the top level main menu
	modes.push(new menu_mode(ren));

	camera displayCamera(0, 0);
	bool resize = true;
	while (!modes.empty())
	{
		//Set up needed variables
		bool animate = true;
		bool redraw = true;
		bool alt_pressed = false;

		while(!modes.empty() && SDL_WaitEvent(&event)){
			do
			{

				// call mode-specific event handler
				if (modes.top()->processEvents(&event, *this))
					delete_top_mode();
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
						resize = true;
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
						case SDLK_F11:
							SDL_ToggleFS(win, ren);
							break;
						case SDLK_LALT:
							alt_pressed = true;
							break;

						case SDLK_RETURN:
							if (alt_pressed) {
								SDL_ToggleFS(win, ren);
							}
							break;
						}
						break;
						case SDL_USEREVENT:
							if (event.user.type == tickSdlEventCode)
								animate = true;
							break;
						case SDL_KEYUP:
						case SDLK_LALT:
							alt_pressed = false;
							break;
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

			if (resize)
			{
				int displayWidth, displayHeight;
				SDL_GetWindowSize(win, &displayWidth, &displayHeight);
				displayCamera = camera(displayWidth, displayHeight);
				redraw = true;
			}

			if (redraw)
			{
				SDL_RenderClear(ren);
				modes.top()->render(ren, displayCamera, font);

				SDL_RenderPresent(ren);
				redraw = false;
			}
		}
	} /* while (!modes.empty()) */

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ /end player one game
	// Cleanup
	SDL_SetCursor(SDL_GetDefaultCursor());
	SDL_FreeCursor(customCursor);
	SDL_RemoveTimer(tickTimerID);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();

	/*system("PAUSE");*/
	return EXIT_SUCCESS;
}
main_class::~main_class()
{
	delete_modes();
}

int main(int argc, char *argv[])
{
	main_class m;
	return m.main_method(argc, argv);
}

// http://stackoverflow.com/a/19054280/2948122
void
space_nomad_SDL_Texture_deleter::operator()(SDL_Texture *texture)
{
	std::cerr << "uniquely freeing " << ((void *)texture) << std::endl;
	SDL_DestroyTexture(texture);
}

// Load a surface
space_nomad_SDL_Surface_unique_ptr loadSurface(const char *filename)
{
	SDL_Surface *bmp = IMG_Load(("images/" + std::string(filename)).c_str());
	if (!bmp)
	{
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		abort();
		return space_nomad_SDL_Surface_unique_ptr();
	}
	return space_nomad_SDL_Surface_unique_ptr(bmp);
}
void
space_nomad_SDL_Surface_deleter::operator()(SDL_Surface *surface)
{
	SDL_FreeSurface(surface);
}
// Load surface then convert to texture and unload surface because of memory leaking. # it isn't that bad though
space_nomad_SDL_Texture_unique_ptr loadTexture(SDL_Renderer *ren, const char *filename)
{
	std::cerr << "Loading " << filename << std::endl;
	auto bmp = loadSurface(filename);

	auto ret = createTexture(ren, bmp);
	std::cerr << "Loaded texture " << filename << " to " << ((void *)ret.get()) << std::endl;
	return ret;
}

space_nomad_SDL_Texture_unique_ptr createTexture(SDL_Renderer *ren, space_nomad_SDL_Surface_unique_ptr& surface)
{
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surface.get());
	if (!tex)
	{
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		abort();
	}
	return space_nomad_SDL_Texture_unique_ptr(tex);
}
space_nomad_SDL_Texture_unique_ptr createTexture(space_nomad_SDL_Renderer_unique_ptr& ren, space_nomad_SDL_Surface_unique_ptr& surface)
{
	return createTexture(ren.get(), surface);
}

void
space_nomad_SDL_Renderer_deleter::operator()(SDL_Renderer *ren)
{
	SDL_DestroyRenderer(ren);
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

// Toggle window modes
// https://forums.libsdl.org/viewtopic.php?t=8777&sid=b6166ccabf631566d1087c8c07251f5d   monkey0506
// Edited by Kristofer Brink
int SDL_ToggleFS(SDL_Window *win, SDL_Renderer* ren)
{
	Uint32 flags = (SDL_GetWindowFlags(win) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (SDL_SetWindowFullscreen(win, flags) < 0) // NOTE: this takes FLAGS as the second param, NOT true/false!
	{
		std::cout << "Toggling fullscreen mode failed: " << SDL_GetError() << std::endl;
		return -1;
	}


	int w = 300;
	int h = 300;
	if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
	{
		SDL_GetWindowSize(win, &w, &h);
		//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		return 1;
	}
	return 0;
}
