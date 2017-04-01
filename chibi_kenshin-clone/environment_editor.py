''' CS 108
Created Fall 2014
environment editor
@author: Kristofer Brink (kpb23)
'''



import pygame, sys
import math
from pygame.locals import *
from block import Block
from camera import Camera
import util
import os
import math
from block_variable import Block_variable
    
class Environment_editor:
    '''
    Models a brush type which is Environment_editor
    '''
    def __init__(self, layers, images_path, images, block_type, variables):
        '''
        Constructor
        (list, list, list, list, list)-> Environment_editor
        '''
        self.files_png = sorted(images)
        self.non_textured = ['Block', 'Triangle', 'Block Top', 'Block Right', 'Block Left', 'Block Bottom', 'Block Invisible', 'Line'] # Type of non textured collideable things. The Top right left and bottom are just blocks with only collision on the specified side
        self.non_textured_surf = util.load('editor/non_textured_selectable.png') # load non   textured image that shows what is being selected
        self.making_tuple = self.non_textured + self.files_png
        self.images = images
        # Sets variables
        self.making_something = False
        self.making_selection_scroll = 0
        self.making_selection = 0
        self.texture_drag_make = False
        # Layer 
        self.layers = layers
        self.layer = self.layers[0]
        self.parallax_variable = Block_variable(1, -2, 4, .01, 'Parallax')
        self.size_variable = Block_variable(1, .1, 5, .01, 'Size')
        self.variable_list = [self.parallax_variable, self.size_variable] + [i[1] for i in variables]
        self.variables = variables
        self.variable_changing = 0
        self.block_type = block_type

        
        
    def closest_corner_lock(self, mouse_x, mouse_y, making):
        ''''
        Locks the cursor to the closest corner of block with similair type and parallax if the user press control
        '''
        try:
            corner_list = []
            for i in self.layer[:-1] if making else self.layer: # the if here makes it so that it ignores the one it is making
                if self.parallax_variable.current == i.parallax: # ignore all that are not on the same parallax layer
                    if i.type.startswith('Block') or not (i.x < i.x2 and i.y > i.y2): # ignore a corner if it is a triangle ////
                        corner_list.append(i.rect.topleft)
                    if i.type.startswith('Block') or not (i.x > i.x2 and i.y > i.y2): # ignore
                        corner_list.append(i.rect.topright)
                    if i.type.startswith('Block') or not (i.x < i.x2 and i.y < i.y2): # ignore
                        corner_list.append(i.rect.bottomleft)
                    if i.type.startswith('Block') or not (i.x > i.x2 and i.y < i.y2): # ignore
                        corner_list.append(i.rect.bottomright)
            lowest_distance = (corner_list[0], ((corner_list[0][0] - mouse_x)**2 + (corner_list[0][1] - mouse_y)**2)) 
            for i in corner_list:
                # Get distance
                distance = (i, ((i[0] - mouse_x)**2 + (i[1] - mouse_y)**2))
                lowest_distance = distance if (lowest_distance[1] > distance[1]) else lowest_distance
                   
            
            return lowest_distance[0]
        except:
            return (mouse_x, mouse_y)
        
        
    def input(self, mouse_x, mouse_y, click, scroll, key_r, key_f, space, ctrl, key_w, key_s, key_a, key_d, key_e, game):
        '''
        Input for Environment editor
        (int, int, bool, int, bool, bool, bool, bool, bool, bool, bool, bool, bool, game)
        '''
        # Scrolling switches what is being made
        if not space:
            if (scroll == -1 or key_f) and self.making_selection < len(self.making_tuple) - 1:
                self.making_selection_scroll += .5 if not key_f else 1
                self.making_something = False
            if (scroll == 1 or key_r) and self.making_selection > 0:
                self.making_selection_scroll -= .5 if not key_r else 1
                self.making_something = False
                
        self.making_selection = int(self.making_selection_scroll) # scroll goes for two ticks so I use this to fix it
       
            
        # change based on parallax
        mouse_x, mouse_y = game.camera.input_xy((mouse_x, mouse_y), self.parallax_variable.current)
        
        # Parallax changing + mouse corner locking
        if space:
            if key_s:
                self.variable_changing = min(self.variable_changing + 1, len(self.variable_list) - 1)
            if key_w:
                self.variable_changing = max(self.variable_changing - 1, 0)
            if key_a != key_d: # exclusive or
                self.variable_list[self.variable_changing].change(key_d)
                
            if key_e:
                self.variable_list[self.variable_changing].reset()
            
            if key_r:
                for variable in self.variable_list:
                    variable.reset()
                
            # Change layers
            # Check if there is more than one layer
            if len(self.layers) > 1 and self.making_something == False:
                if self.variable_changing == 0:
                    if self.variable_changing == 0:
                        if self.variable_list[0].current < 1:
                            self.layer = self.layers[0]
                        if self.variable_list[0].current > 1:
                            self.layer = self.layers[1]
#                         
#                 if space: # Switching between type of layers
#                     self.current_layer_number += 1
#                     self.current_layer_number %= len(self.layers)
#                     self.layer = self.layers[self.current_layer_number]
#                     
#                     self.making_something = False
#                 
        
        # Changes the making to the tuple
        making = self.making_tuple[self.making_selection]
        # Either makes a new block or a surface
        if click: 
            if self.making_selection < len(self.non_textured):   
                
                if click and self.layer and ctrl:
                    mouse_x, mouse_y = self.closest_corner_lock(mouse_x, mouse_y, self.making_something) # lock to closest corner
                    
                if self.making_something == False:
                        self.layer.append(self.block_type(game.texture_cache, mouse_x, mouse_y,  block_type = making, parallax=self.parallax_variable.current, **{key: value.current for (key, value) in self.variables}))  # http://stackoverflow.com/questions/334655/passing-a-dictionary-to-a-function-in-python-as-keyword-parameters
                        self.making_something = True
                else:
                    if (self.layer[-1].height < 2 or self.layer[-1].width < 2) and self.layer[-1].type != 'Line': # Deletes block if too small
                        self.layer.pop(len(self.layer)-1)
                    self.making_something = False
                        
        if not self.making_selection < len(self.non_textured): # if not making a block then it is making textured block
            left_click = pygame.mouse.get_pressed()[0]
            if click:
                
                self.layer.append(self.block_type(game.texture_cache, mouse_x, mouse_y, making, parallax=self.parallax_variable.current, size = self.size_variable.current, **{key: value.current for (key, value) in self.variables})) # https://docs.python.org/2/library/functions.html#getattr
                    
            if left_click: # Continue making when dragging mouse
                if self.layer: # make sure layer is not empty
                    x_spot, y_spot = mouse_x - self.layer[-1].rect.width//2, mouse_y - self.layer[-1].rect.height//2
                    rect_new = pygame.Rect((x_spot, y_spot) + (self.images[making].get_size()[0] * self.size_variable.current, self.images[making].get_size()[1] * self.size_variable.current))
                    if not self.layer[-1].rect.colliderect(rect_new):
                        rect_init = self.layer[-1].rect
                        if rect_new.left > rect_init.right:
                            rect_new.left = rect_init.right
                        if rect_new.right < rect_init.left:
                            rect_new.right = rect_init.left
                        if rect_new.top > rect_init.bottom:
                            rect_new.top = rect_init.bottom
                        if rect_new.bottom < rect_init.top:
                            rect_new.bottom = rect_init.top
                        new_x_spot, new_y_spot = rect_new.center
                        self.layer.append(
                                          self.block_type(
                                                          game.texture_cache,
                                                          new_x_spot,
                                                           new_y_spot,
                                                            making,
                                                             parallax=self.parallax_variable.current,
                                                              size = self.size_variable.current,
                                                               **{key: value.current for (key, value) in self.variables})) # https://docs.python.org/2/library/functions.html#getattr
        
        
                    
    def logic(self, mouse_x, mouse_y, click, scroll, ctrl, game):
        '''
        Runs logic for environment editor
        (int, int, bool, int, bool, game)
        '''
        # change based on parallax
        self.mouse_pos = game.camera.input_xy((mouse_x, mouse_y), self.parallax_variable.current)
                
        # Logic for either the block that is being made or logic for the position of a texture centered at mouse position
        if self.making_selection < len(self.non_textured): # square or triangle
            
            # lock to closest corner
            if ctrl and len(self.layer) > 1:
                self.mouse_pos = self.closest_corner_lock(self.mouse_pos[0], self.mouse_pos[1], self.making_something) # lock to closest corner
                
            if self.making_something == True:
                self.layer[len(self.layer)-1].make(self.mouse_pos[0], self.mouse_pos[1], self.parallax_variable.current)
                
                
    def draw(self, draw, screen, game):
        '''
        Draw things specific to environment editor like block variables and images that can be scrolled through on the right side of the screen
        '''
        self.screen_width, self.screen_height = screen.get_size() # Get screen height and width
        making = self.making_tuple[self.making_selection]
        # Draw texture being made if it is being made
        if not self.making_selection < len(self.non_textured):
            self.block_type.editor_draw(
                                    screen, 
                                   game.camera.camera_xy(self.mouse_pos, parallax=self.parallax_variable.current),
                                   self.images[making],
                                   self.parallax_variable.current,
                                   self.size_variable.current,
                                   **{key: value.current for (key, value) in self.variables}) # http://stackoverflow.com/questions/1747817/python-create-a-dictionary-with-list-comprehension, http://stackoverflow.com/questions/3294889/iterating-over-dictionaries-for-loops-in-python
             
            
        '''Show whether the user is putting things into the foreground or background''' # Not doing it this way anymore. There are better ways of doing things
        '''
        
        if self.layer_surf is not None:
            surf_width = self.layer_surf.get_size()[0]
            x_spot = self.screen_width/2 - surf_width//2
            y_spot = int(self.screen_height * .9)
            screen.blit(self.layer_surf , (x_spot, y_spot) )   I might need the x and y spot for later
            '''
        '''SHow the variables that are at the bottom of the screen'''
        
        x_spot = self.screen_width/2 - 145
        y_spot = int(self.screen_height - 200)
        count = 0
        for variable in self.variable_list:
            variable.draw(screen, (x_spot, y_spot), (0,0,0) if count != self.variable_changing else (255,0,255))
            y_spot += 25
            count += 1
            
        ''' Draw the side that shows what they are switching through'''
        # Non textured ones
        subtraction_var = self.making_selection * 125
        screen.blit(self.non_textured_surf, ( self.screen_width - 120, self.screen_height/2 - 100 - subtraction_var))
            
            
        # Textured ones
        count = len(self.non_textured) - 1
        for i in self.files_png:
            count += 1
            surface = self.images[i]
            subtraction_var = self.making_selection * 125 - count * 125
            screen.blit(pygame.transform.scale(surface, (100, 100)), ( self.screen_width - 120, self.screen_height/2 - 100 - subtraction_var))
            
                         
            