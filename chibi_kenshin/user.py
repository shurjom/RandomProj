''' CS 108
Created Fall 2014
user
@author: Kristofer Brink (kpb23)
'''


import pygame, sys
import math
from pygame.locals import *
import rectangle
from player import Player
from pygame.locals import *

class User(Player):
    '''
    Models the User and is subclass of Player
    
    '''
    def __init__(self, texture_cache):
        '''
            (Texture_Cache) -> User(Player)
        '''
        self.idle_surf = texture_cache.load('sprites/user/kenshin_idle.png')
        
        
        width, height = self.idle_surf.get_size()
        Player.__init__(self, x=0, y=0, width=width, height=height, ground_accelerate=.99,ground_decelerate=.5,
                        air_acclerate=.6, air_decelerate=.95,air_antigravity=.5,air_downards=2,
                        wall_decelerate=.7, wall_jump_dy=-16, wall_jump_dx=17, 
                        jump_speed=-18, 
                        capped_dx_ground=15,capped_dx_air=10,capped_dx=30,capped_dy=35,
                        gravity=1)
        
        self.direction ='Right'
        
        
    def input(self, up, down, left, right):
        '''
        (bool, bool, bool, bool)
        Handles input for user
        '''
        Player.input(self, up, down, left, right)
        if right and not left:
            self.direction = 'Right'
        if left and not right:
            self.direction = 'Left'
            
            
        
    
    
    def draw(self, screen, camera):
        '''
        Draws the user
        (screen, Camera)
        '''
        if self.direction == 'Right':
            screen.blit(self.idle_surf, camera.camera_xy((self.rect.rect.x, self.rect.rect.y)))
        else:
            screen.blit(pygame.transform.flip(self.idle_surf, True, False), camera.camera_xy((self.rect.rect.x, self.rect.rect.y)))
        