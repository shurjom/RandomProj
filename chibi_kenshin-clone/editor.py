''' CS 108
Created Fall 2014
editor
@author: Kristofer Brink (kpb23)
'''



import pygame, sys
import math
from pygame.locals import *
import player
from game import GameGameMode
from block import Block
from camera import Camera
from editor_menu import Editor_menu
import util
import os

        
    
def load_images(game, path, texture_cache):
    ''' 
    Loads image for the editor to use
    '''
    files = sorted(os.listdir('images/' + path))
    files_png = [i for i in files if i.endswith('.png')]
    images = {}
    for i in files_png:
        new_path = path + i
        images[new_path] = texture_cache.load(new_path)
    return images

    
class EditorGameMode(GameGameMode):
    '''
    Models a game mode that is for editing
    '''
    def __init__(self, menuGameMode, file_name = None): # http://tinyurl.com/superclasspython
        '''
        (gamemode, str)-> EditorGameMode(GameGameMode)
        '''
        super(EditorGameMode, self).__init__(menuGameMode, file_name)
        self.menuGameMode = menuGameMode
        pygame.mouse.set_visible(True)
        
        
        self.scenery_images = load_images(self, 'scenery/still/', self.texture_cache) # load images and lists for places
        self.collidable_images = load_images(self, 'collidable/', self.texture_cache)
        
        self.editor_menu = Editor_menu(self)
        
        self.block_making = False
        self.up_w = self.down_s = self.left_a = self.right_d = self.shift = False # movement keys
        self.q = self.e = self.r = self.f = self.click = self.ctrl = False # Editing keys and mouse click
        self.mouse_x = self.mouse_y = self.mouse_i_x = self.mouse_i_y = 0
        self.mouse_scroll = 0
        self.key_space = False # switches between options in making things
        self.key_escape = False# leave the editor
        self.delete_rect_size = 50
        
    def handleEvent(self, event):
        '''Handles keyboard and mouse events'''
        self.mouse_x, self.mouse_y = pygame.mouse.get_pos()
        
        self.click = False
        self.key_escape = False
        
        # Handle mouse events
        if event.type == pygame.MOUSEBUTTONDOWN:
            # Left click
            if event.button == 1:
                self.click = True
                
            self.mouse_scroll = 0
            
            if event.button == 4: # scrolling up and down
                self.mouse_scroll = 1
            if event.button == 5:
                self.mouse_scroll = -1
                
        right_click = pygame.mouse.get_pressed()[2]
        # Right click
        if right_click:
            self.editor_menu.collidable.making_something = False
            self.editor_menu.scenery.making_something = False
            # Delete everything that pointer is over if right click
            deletable_list = [self.block, self.scenery_foreground, self.scenery_background]
            for deletable in deletable_list:
                for i in reversed(range(len(deletable))): # reversed to avoid index error
                    # Take into account parallax
                    x_delete, y_delete = self.camera.input_xy((self.mouse_x, self.mouse_y), deletable[i].parallax)
                    delete_rect = (x_delete - self.delete_rect_size/2, y_delete - self.delete_rect_size/2, self.delete_rect_size, self.delete_rect_size)
                    
                    if deletable[i].rect.colliderect(delete_rect): # Using collide rectangle with size ten for thin boxes that are hard to delete/ super small
                        if deletable[i].type == 'Triangle':
                            ''' Specific collision detection for a triangle'''
                            if util.triangle_collide(deletable[i], delete_rect):
                                deletable.pop(i)
                        else:
                            deletable.pop(i) # The sailer man

        
        # WASD keys events + shift key and ctrl
        if event.type == KEYDOWN:
            if event.key == K_w:
                self.up_w = True
            if event.key == K_s:
                self.down_s = True
            if event.key == K_a:
                self.left_a = True
            if event.key == K_d:
                self.right_d = True
            
            if event.key == K_LSHIFT:
                self.shift = True
            if event.key == K_LCTRL:
                self.ctrl = True
                
            if event.key == K_SPACE:
                self.key_space = True
                
            if event.key == K_ESCAPE:
                self.key_escape = True
                
        if event.type == KEYUP:
            if event.key == K_w:
                self.up_w = False
            if event.key == K_s:
                self.down_s = False
            if event.key == K_a:
                self.left_a = False
            if event.key == K_d:
                self.right_d = False
        
            if event.key == K_LSHIFT:
                self.shift = False
            if event.key == K_LCTRL:
                self.ctrl = False
            
            if event.key == K_SPACE:
                self.key_space = False
                
        # Editor menu keys
        self.q = self.e = self.r = self.f = False
        
        if event.type == KEYDOWN:
            if event.key == K_q:
                self.q = True
            if event.key == K_e:
                self.e = True
            if event.key == K_r:
                self.r = True
            if event.key == K_f:
                self.f = True
                
        new_mode = self.editor_menu.input(self.key_escape, self.q, self.e, self.r, self.f, self.key_space, self.ctrl, self.up_w, self.down_s, self.left_a, self.right_d, self.mouse_x, self.mouse_y, self.click, self.mouse_scroll, self) # EDITOR EDITING THINGS
        
        if new_mode is not None:
            return new_mode
        
        
        return super(EditorGameMode, self).handleEvent(event) # Has to return
            
    def logic(self):
        '''
        Logic for editor
        '''
        super(EditorGameMode, self).logic()
        #camera
        if not self.key_space: # don't move when changing options
            self.camera.cam_editor_move(self.up_w, self.down_s, self.left_a, self.right_d, self.shift)
        
        self.editor_menu.logic(self.mouse_x, self.mouse_y, self.click, self.mouse_scroll, self.ctrl, self) # EDITOR EDITING THINGS
        
        
        self.mouse_scroll = 0
        
        
        
    def draw(self, draw, screen):
        '''
        Draws for editor
        '''
        super(EditorGameMode, self).draw(draw, screen, editor=True)
        self.editor_menu.draw(draw, screen, self)
        
        
        
        
