#ifndef _TESTSDL_MAIN_HXX
#define _TESTSDL_MAIN_HXX

#include <memory>
#include <stack>
#include <sstream>
#include <string> 

extern "C" {
#include <SDL.h>
}

class mode;

class main_class
{
private:
	std::stack<mode *> modes;
	void delete_modes();
	void delete_top_mode();
public:
	int main_method(int argc, char *argv[]);
	void push_mode(mode *mode);
	virtual ~main_class();

	SDL_Renderer *ren;
	SDL_Window *win;
};

#define SPACE_NOMAD_ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

extern int space_nomad_fps;

//Number to string done  Copyright © 2014 Nathan Phillip Brink <ohnobinki@ohnopublishing.net>
template <class T> static std::string str(const T &o)
{
  std::stringstream sb;
  sb << o;
  return sb.str();
}

class space_nomad_SDL_Texture_deleter
{
public:
	void operator()(SDL_Texture *texture);
};
typedef std::unique_ptr<SDL_Texture, space_nomad_SDL_Texture_deleter> space_nomad_SDL_Texture_unique_ptr;
extern  space_nomad_SDL_Texture_unique_ptr loadTexture(SDL_Renderer *ren, const char *filename);

class space_nomad_SDL_Surface_deleter
{
public:
	void operator()(SDL_Surface *surface);
};
typedef std::unique_ptr<SDL_Surface, space_nomad_SDL_Surface_deleter> space_nomad_SDL_Surface_unique_ptr;
space_nomad_SDL_Surface_unique_ptr loadSurface(const char *filename);
extern  space_nomad_SDL_Texture_unique_ptr createTexture(SDL_Renderer *ren, space_nomad_SDL_Surface_unique_ptr& surface);

class space_nomad_SDL_Renderer_deleter
{
public:
	void operator()(SDL_Renderer *ren);
};
typedef std::unique_ptr<SDL_Renderer, space_nomad_SDL_Renderer_deleter> space_nomad_SDL_Renderer_unique_ptr;
extern  space_nomad_SDL_Texture_unique_ptr createTexture(space_nomad_SDL_Renderer_unique_ptr& ren, space_nomad_SDL_Surface_unique_ptr& surface);

#endif /* _TESTSDL_MAIN_HXX */
