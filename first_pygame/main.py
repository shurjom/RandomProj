import pygame, sys
from pygame.locals import *
import player

def main():
    
    pygame.init()
    fpsClock = pygame.time.Clock()
    
    windowSurface = pygame.display.set_mode((1400, 800))
    pygame.display.set_caption('TAG')
    
    whiteColor = pygame.Color(255, 255, 255)
    
    mousex = mousey = 0
    up = down = left = right = False
    myPlayer = player.player(24, 24, 24, pygame.Color(255, 127, 0))
    
    while True:
        windowSurface.fill(whiteColor)
        
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                return
            if event.type == KEYDOWN:
                if event.key == K_UP:
                    up = True
                if event.key == K_DOWN:
                    down = True
                if event.key == K_LEFT:
                    left = True
                if event.key == K_RIGHT:
                    right = True
            if event.type == KEYUP:
                if event.key == K_UP:
                    up = False
                if event.key == K_DOWN:
                    down = False
                if event.key == K_LEFT:
                    left = False
                if event.key == K_RIGHT:
                    right = False
        
        myPlayer.playerLogic(up, down, left, right)
        
        myPlayer.circle.draw(pygame.draw, windowSurface)
        
        
        pygame.display.update()
        # for some reason, calling fpgClock.tick(60) causes a periodic
        # jerkiness in motion. So, we just assume the computer is fast enough
        # to run our logic that the extra processing time is not observable.
        pygame.time.delay(int(1000/60))

if __name__ == "__main__":
    main()

