''' CS 108
Created Fall 2014
game
@author: Kristofer Brink (kpb23)
'''




import pygame, sys
import math
from pygame.locals import *
import player
from camera import Camera
import util
import os
import json
import block
import texture_cache
from user import User

class GameGameMode(object):
    '''
    Models a type of game mode
    Invariants:
        file_name is string
        menuGameMode is object
    '''
    def __init__(self, menuGameMode, file_name = None):
        '''
        Constructor
        (object, str)-> GameGameMode
        '''
        pygame.mouse.set_visible(False) # Make the mouse invisible
        self.menuGameMode = menuGameMode
        self.up = self.down = self.left = self.right = False
        self.block = []
        self.scenery_foreground = []
        self.scenery_background = []
        self.inGame = False
        
        self.texture_cache = texture_cache.Texture_cache()
        
        
        self.myUser = User(self.texture_cache) #(x, y, width, height, color)
        
        
        
        
        self.file_name = file_name
        if file_name is not None:
            self.load_game(file_name)
            
            
        self.camera = Camera(self.myUser.rect.rect)
        
        self.camera.cam_take_screen_size(pygame.display.get_surface()) # Check size of screen
        
        self.camera.cam_take_screen_size(pygame.display.get_surface()) # Take in screen size to camera
        
    def handleEvent(self, event):
        '''
        Handles keyboard and mouse events
        (event)
        '''
        if event.type == KEYDOWN:
            if event.key == K_UP:
                self.up = True
            if event.key == K_DOWN:
                self.down = True
            if event.key == K_LEFT:
                self.left = True
            if event.key == K_RIGHT:
                self.right = True
            if event.key == K_g:
                return self.menuGameMode
            
            if event.key == K_ESCAPE:
                
                return self.menuGameMode
        
        if event.type == KEYUP:
            if event.key == K_UP:
                self.up = False
            if event.key == K_DOWN:
                self.down = False
            if event.key == K_LEFT:
                self.left = False
            if event.key == K_RIGHT:
                self.right = False
        
        # Change camera mode to player when player presses arrow keys
        if self.up or self.down or self.left or self.right:
            self.camera.cam_game = True
                     
    def logic(self):
        '''
        logic of game mode
        '''
        if self.camera.cam_game:
            self.myUser.playerLogic(self.up, self.down, self.left, self.right) # Camera is in camera game
            
            self.camera.cam_game_move(self.myUser.rect.rect) #Parallax into collision
            self.myUser.rect.block_collision(self.block, self) # Collision
            self.camera.cam_game_move(self.myUser.rect.rect) # Get camera position after all player collision logic
            
        
    def draw(self, draw, screen, editor=False):
        ''' 
        Drawing for game mode
        '''
        screen.fill(pygame.Color(255, 255, 255))
        # Draw background scenery
        for i in self.scenery_background:
            i.draw(pygame.draw, screen, self.camera, True)
        
        
        # Draw the blocks
        for i in self.block:
            i.draw(pygame.draw, screen, self.camera, editor)
        
        self.myUser.draw(screen, self.camera)
        
        # Draw foreground scenery
        for i in self.scenery_foreground:
            i.draw(pygame.draw, screen, self.camera, True)
            
    def to_dictionary(self):
        ''' 
        Change to dictionary for saving into txt file
        '''
        return {
                'block' : [block.to_dictionary() for block in self.block],
                'scenery_foreground' : [scenery.to_dictionary() for scenery in self.scenery_foreground],
                'scenery_background' : [scenery.to_dictionary() for scenery in self.scenery_background]
                }

    def load_game(self, file_name):
        '''
        Loads from a txt file
        (str)
        '''
        if file_name is not None:
            with open(file_name) as file:
                self.from_dictionary(json.loads(file.read()))

    def from_dictionary(self, dictionary):
        '''
        Load level from a dictionary
        '''
        self.block = [block.Collidable_block(self.texture_cache, **{key: value for (key, value) in i.items()}) for i in dictionary['block']]
        self.scenery_background = [block.Scenery_block(self.texture_cache, **{key: value for (key, value) in i.items()}) for i in dictionary['scenery_background']]
        self.scenery_foreground = [block.Scenery_block(self.texture_cache, **{key: value for (key, value) in i.items()}) for i in dictionary['scenery_foreground']]
    
    

