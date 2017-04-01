''' CS 108
Created Fall 2014
Runs the game
@author: Kristofer Brink (kpb23)

Tests are in:
    block_variable
    block
    camera
    
Inheritance is used in:
    block
    user
    editor
'''
# made by Kristofer Brink
import pygame, sys, os
from pygame.locals import *
from menu import MenuGameMode
import util


def main():
    
    os.environ['SDL_VIDEO_CENTERED'] = '1'  # centers Pygame SCREEN on desktop got it from http://stackoverflow.com/questions/10867402/pygame-using-shift-when-entering-name
    pygame.init()
    fpsClock = pygame.time.Clock()      

    screen = pygame.display.set_mode((1400, 900), HWSURFACE|DOUBLEBUF|RESIZABLE)
    pygame.display.set_caption('Chibi Kenshin')
    pygame.display.set_icon(util.load('icon.png'))
    currentGameMode = MenuGameMode() # My cousin showed me a good way of handling different game modes and also having the menu that 'remembers' stuff. I used this before just using it again
    
    while True:
        for event in pygame.event.get():
            # Quit
            if event.type == QUIT:
                pygame.quit()
                return
            
            # Window resizing   http://pygame.org/wiki/WindowResizing
            elif event.type == VIDEORESIZE:
                screen = pygame.display.set_mode(event.dict['size'],HWSURFACE|DOUBLEBUF|RESIZABLE)
                
                if hasattr(currentGameMode, 'camera'): # Only change the screen size when resized https://docs.python.org/2/library/functions.html#hasattr
                    currentGameMode.camera.cam_take_screen_size(screen)
                    
            else:
                if event.type == KEYDOWN:
                    if event.key == K_F11: 
                        full_screen_modes = pygame.display.list_modes() # http://www.pygame.org/docs/ref/display.html#pygame.display.list_modes
                        if screen.get_flags() & FULLSCREEN:
                            pygame.display.set_mode((1400, 900),HWSURFACE|DOUBLEBUF|RESIZABLE)
                            
                        elif full_screen_modes != -1 and len(full_screen_modes) > 0: 
                            best_mode = full_screen_modes[0]
                            pygame.display.set_mode(best_mode, FULLSCREEN)
                            
                    
                currentGameMode = currentGameMode.handleEvent(event) or currentGameMode
                
                if currentGameMode == 'Quit':
                    pygame.quit()
                    return
        
        currentGameMode.logic()
        
        currentGameMode.draw(pygame.draw, screen)
        
        pygame.display.flip()
        fpsClock.tick(60)
        
        

if __name__ == "__main__":
    main()
    
    
    