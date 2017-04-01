from pygame.locals import *
from pygame import font
from util import draw_center

class InputMode:
    def __init__(self, original_mode, background_draw, text_callback, surface, restrict_to_letters=False, orig_string=None):
        self.text = '' or orig_string
        self.original_mode = original_mode
        self.background_draw = background_draw
        self.surface = surface
        self.myfont = font.SysFont("consolas", 17)
        self.text_callback = text_callback
        self.restrict_to_letters = restrict_to_letters
        
    def handleEvent(self, event):
        # http://stackoverflow.com/questions/14224518/what-is-a-good-way-to-get-keyboard-input-in-python-pygame
        if event.type == KEYDOWN:
            if event.key == K_ESCAPE:
                return self.original_mode
            if event.key == K_RETURN:
                self.text_callback(self.text)
                return self.original_mode
            if event.key == K_BACKSPACE:
                self.text = self.text[:-1]
            
            self.text = self.text + (event.unicode if (event.unicode.isalpha() or not self.restrict_to_letters or event.unicode == ' ') else '')

    def logic(self):        
        pass # https://groups.google.com/forum/#!topic/comp.lang.python/7dw3ZxbfICk

    def draw(self, draw, screen):
        self.background_draw(draw, screen)
        text_surf = self.myfont.render(self.text, 1, (0,0,0)) # Number of things made in the collidable
        width, height = self.surface.get_size()
        draw_center(self.surface, screen, draw, width, height)
        draw_center(text_surf,screen, draw, width - 20, 0)
        
        