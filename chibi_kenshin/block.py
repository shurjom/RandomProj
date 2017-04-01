''' CS 108
Created Fall 2014
block
@author: Kristofer Brink (kpb23)
'''



import math
import pygame, sys
from pygame.locals import *
import rectangle
from builtins import staticmethod


class Block(object):
    '''
    Models a block
    Invariants:
        x is int
        y is int
        surface is pygame surface
        block_type is string
        parallax is float
        size is float 
        rotation is int
        x2 is int
        y2 is int
        parallax_2 is float
    '''
    def __init__(self, texture_cache, x, y, surface=None, block_type = 'Block', parallax = 1, size = 1.0, rotation = 0.0, x2=None, y2=None, parallax_2 = None):
        '''
        Constructor
        (Texture_cache, int, int, surface, str, float, float, int, int, int, float) -> Block
        
        '''
        self.parallax = parallax
        self.new_point = (x,y)
        # Set same values for other to avoid null error
        self.x2 = x 
        self.y2 = y 
        
        
        self.rect = pygame.Rect(x, y, 0, 0)
        self.width, self.height = (1,1)
        self.size = size
        self.surface_directory = surface
    
        self.use_surface = surface is not None
        if self.use_surface:
            
            surface = texture_cache.load(surface)
            width, height = surface.get_size()
            if size != 1.0:
                width = int(width * size)
                height = int(height * size)
                surface = pygame.transform.scale(surface, (width, height))
            if rotation != 0.0:
                surface = pygame.transform.rotate(surface, rotation)
                width, height = surface.get_size()
            # Center
            x -= width//2
            y -= height//2
            self.x2 += width//2
            self.y2 += height//2
            
            self.rect = pygame.Rect(x, y, width, height)
            self.surface = surface
            
        # Block making options
        self.type = block_type
        self.x_min = self.x = x
        self.y_min = self.y = y
        
        parallax_2 = parallax_2

        if x2 is not None and y2 is not None: # this is for loading levels
            self.make(x2,y2, parallax_2)
        
    def make(self, x2=0, y2=0, parallax = None): # Only used for non surface block
        ''' For making a Block'''
        # Put the new thing into pygame rectangle
        self.x2 = x2
        self.y2 = y2
        # do math to make the mouse input make sense
        # Use list to find smallest x and y
        self.x_min = min([self.x, self.x2])
        self.y_min = min([self.y, self.y2])
        # Store width and height
        self.width = abs(self.x - self.x2)
        self.height = abs(self.y - self.y2)
        
        if self.type == 'Triangle':
            self.new_point = (self.x2, self.y)
        
        if self.type == 'Line':
            self.x2, self.y2 = (x2, y2)
            self.parallax_2 = parallax
        
        self.rect = pygame.Rect(self.x_min, self.y_min, self.width, self.height)
        
        
    @staticmethod
    def editor_draw(screen, position, surface, parallax, size): # http://stackoverflow.com/questions/735975/static-methods-in-python
        ''' Draw for editor is a static method, used for drawing where the block is going to be'''
        size = tuple([int(i * size) for i in surface.get_size()])
        screen.blit(pygame.transform.scale(surface, size), (position[0] - size[0]//2, position[1] - size[1]//2))

        
    def draw(self, draw, screen, camera, editor=False):
        ''' 
        Draws the Block
        '''
        position = (camera.camera_xy((self.x_min, self.y_min), self.parallax))
        color = pygame.Color('Black')
        if self.use_surface:
            # draw surface
            screen.blit(self.surface, position)
            return
        
        if self.type == 'Block':
            # draw rectangle to screen
            draw.rect(screen, color, (position[0], position[1], self.width, self.height))
            return
            
        if self.type == 'Triangle':
            # draw triangle 
            draw.polygon(screen, color, [camera.camera_xy((self.x, self.y), self.parallax), camera.camera_xy(self.new_point, self.parallax), camera.camera_xy((self.x2, self.y2), self.parallax)])
            return
            
        # Set up color for indicating the side that is not collidable with
        not_collide_color = pygame.Color(200, 200, 200)
        half_height = math.ceil(self.height/2)
        half_width = math.ceil(self.width/2)
        
        
        if self.type == 'Block Top':
            draw.rect(screen, color, (position[0], position[1], self.width, half_height))
            draw.rect(screen, not_collide_color, (position[0], position[1] + half_height, self.width, half_height))
            
        if self.type == 'Block Bottom':
            draw.rect(screen, color, (position[0], position[1] + half_height, self.width, half_height))
            draw.rect(screen, not_collide_color, (position[0], position[1], self.width, half_height))
        
        if self.type == 'Block Right':
            draw.rect(screen, color, (position[0], position[1], half_width, self.height))
            draw.rect(screen, not_collide_color, (position[0] + half_width, position[1], half_width, self.height))
            
        if self.type == 'Block Left':
            draw.rect(screen, color, (position[0]  + half_width, position[1], half_width, self.height))
            draw.rect(screen, not_collide_color, (position[0], position[1], half_width, self.height))
            
        if editor and self.type == 'Block Invisible':
            # Draw the block but in a different color and if making scenery then just draws that same type of color in the game mode
            draw.rect(screen, pygame.Color(150,150,150), (position[0], position[1], self.width, self.height))
            
        if self.type == 'Line':
            line_x2, line_y2 = (camera.camera_xy((self.x2, self.y2), self.parallax_2))
            line_x, line_y = (camera.camera_xy((self.x, self.y), self.parallax))
            draw.line(screen, color, (line_x, line_y), (line_x2, line_y2), 4)
        
            
    def to_dictionary(self):
        '''
        Makes a dictionary of the block in order to make is savable into a txt file
        '''
        block_dict = {
                'x' : self.x if not self.use_surface else self.x + self.surface.get_size()[0]//2, 
               'y': self.y if not self.use_surface else self.y + self.surface.get_size()[1]//2, 
               'surface' : self.surface_directory,
               'block_type' : self.type, 
               'parallax' : self.parallax,
                'size' : self.size,
                'x2' : self.x2,
                'y2' : self.y2
               }
        if self.type == 'Line':
            block_dict['parallax_2'] = self.parallax_2
        
        return block_dict
        
        
        
        
class Collidable_block(Block):
    '''
    Models a Collidable_block(Block)
    Is subclass of Block
    Invariants:
        jump is float
        ground_friction is float
        wall_friction is float
        wall_jump_up is float
        wall_jump_sideways is float
    
    '''
    def __init__(self, texture_cache, x, y, surface=None, block_type = 'Block', parallax = 1, parallax_2 = None, size = 1.0, jump = 1, ground_friction = 1, wall_friction = 1, wall_jump_up = 1, wall_jump_sideways = 1, x2=None, y2=None):
        '''
        (Same as block, int, int, int, int, int)-> Collidable_block(Block)
        '''
        Block.__init__(self, texture_cache, x, y, surface, block_type, parallax, size, x2=x2, y2=y2, parallax_2=parallax_2)
        self.jump = jump
        self.ground_friction = ground_friction
        self.wall_friction = wall_friction
        self.wall_jump_up = wall_jump_up
        self.wall_jump_sideways = wall_jump_sideways
        
    def to_dictionary(self):
        '''Saves block properties as a dictionary'''  # http://stackoverflow.com/questions/38987/how-can-i-merge-two-python-dictionaries-in-a-single-expression
        return dict(Block.to_dictionary(self).items() |
                {
                'jump' : self.jump, 
                'ground_friction' : self.ground_friction, 
                'wall_friction' : self.wall_friction, 
                'wall_jump_up' : self.wall_jump_up, 
                'wall_jump_sideways' : self.wall_jump_sideways}.items())
        
        
    @staticmethod
    def editor_draw(screen, position, surface, parallax, size, jump = 1, ground_friction = 1, wall_friction = 1, wall_jump_up = 1, wall_jump_sideways = 1):
        ''' For drawing in in the editor what the block would look like if it was placed'''
        Block.editor_draw(screen, position, surface, parallax, size)
        
        
class Scenery_block(Block):
    '''
    Models a Scenercy_block(Block):
    Is sublcass of Block
    Invariants:
        Same as block
    '''
    
    def __init__(self,texture_cache, x, y, surface=None, block_type = 'Block', parallax = 1, size = 1.0, rotation = 0, x2=None, y2=None, parallax_2 = None):
        Block.__init__(self, texture_cache, x, y, surface, block_type, parallax, size, rotation, x2=x2, y2=y2, parallax_2=parallax_2)
        self.rotation = rotation

    def to_dictionary(self):
        '''Makes scenery block to dictionary for saving purposes'''
        return dict(Block.to_dictionary(self).items()|
        {
         'rotation' : self.rotation
         }.items())


    @staticmethod
    def editor_draw(screen, position, surface, parallax, size, rotation): # http://stackoverflow.com/questions/735975/static-methods-in-python
        ''' For drawing in in the editor what the block would look like if it was placed'''
        Block.editor_draw(screen, position, pygame.transform.rotate(surface, rotation), parallax, size)
        
        
